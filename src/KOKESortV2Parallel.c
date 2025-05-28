#include "KOKESortV2Parallel.h"

/**
 * @brief Thread worker function: process a subset of elements to count occurrences in index buckets.
 *
 * @param arg Pointer to ThreadArgs struct
 * @return int Returns 0 on success, -1 on error (out-of-bound index)
 */
static void *thread_counting(void *arg)
{
    CountThreadArgs *args = (CountThreadArgs *)arg;

    for (size_t i = 0; i < args->processCount; ++i)
    {
        // Calculate pointer to current element in the array chunk
        void *element = (char *)args->startPoint + i * args->elementSize;

        // Get index for current element using user-provided function
        size_t index = (size_t)args->indexerFn(element, NULL);

        // Validate index does not exceed maximum allowed index
        if (index >= args->pbSpaceCount)
        {
            // Set shared error flag atomically if invalid index detected
            K_LOG_ERROR("Thread %lu: indexerFn returned out-of-bounds index: " SIZE_T_IDENTIFIER, cthreads_thread_id(cthreads_thread_self()), index);
            ATOMIC_STORE_SIZE_T(args->errorFlag, true);
            return (void *)-1;
        }

        // Atomically increment the count for this index
        ATOMIC_ADD_SIZE_T(&args->pbSpaces[index].Capacity, 1);
    }

    return (void *)0;
}

/**
 * @brief Parallel counting function: counts occurrences of elements in different index buckets.
 *
 * @param array Pointer to input array start
 * @param arraySize Number of elements in input array
 * @param elementSize Size of each element in bytes
 * @param pbSpaceCount Number of possible index buckets
 * @param indexerFn User-defined function mapping an element to an index
 * @param counts Pre-allocated atomic array of counts (size pbSpaceCount)
 * @return true on success, false on failure or invalid input
 */
bool CountPossibilitySpaces_Parallel(
    void *array,
    size_t arraySize,
    size_t elementSize,
    size_t pbSpaceCount,
    PossibilitySpace *pbSpacesStoreArray,
    SpaceIndexerFn indexerFn,
    void *indexerFnData,
    size_t threadCount,
    unsigned long threadTimeoutMs)
{
    // --- Input validation starts here ---
    if (array == NULL)
    {
        K_LOG_ERROR("Input 'array' pointer is NULL.");
        return false;
    }

    if (arraySize == 0)
    {
        K_LOG_ERROR("Input 'arraySize' is zero.");
        return false;
    }

    if (elementSize == 0)
    {
        K_LOG_ERROR("Input 'elementSize' is zero.");
        return false;
    }

    if (pbSpaceCount == 0)
    {
        K_LOG_ERROR("Input 'pbSpaceCount' is zero.");
        return false;
    }

    if (pbSpacesStoreArray == NULL)
    {
        K_LOG_ERROR("Input 'pbSpacesStoreArray' pointer is NULL.");
        return false;
    }

    if (indexerFn == NULL)
    {
        K_LOG_ERROR("Input 'indexerFn' pointer is NULL.");
        return false;
    }

    /*
    if(indexerFnData == NULL)
    {
        K_LOG_ERROR("Input 'indexerFnData' pointer is NULL.");
        return false;
    }
    */

    if (threadCount == 0)
    {
        K_LOG_ERROR("Input 'threadCount' is zero.");
        return false;
    }

    if (threadTimeoutMs == 0)
    {
        K_LOG_ERROR("Input 'threadTimeoutMs' is zero.");
        return false;
    }
    // --- Input validation ends here ---

    /*
    // Initialize counts array atomically to zero before starting threads
    for (size_t i = 0; i < pbSpaceCount; ++i)
    {
        ATOMIC_STORE_SIZE_T(&counts[i], 0);
    }
    */

    size_t argsSize = sizeof(CountThreadArgs) * threadCount;
    CountThreadArgs *args = (CountThreadArgs *)AllocFn(argsSize);

    if (args == NULL)
    {
        K_LOG_ERROR("Failed to allocate memory for thread arguments.");
        return false;
    }

    size_t threadsSize = sizeof(cthread_t) * threadCount;
    cthread_t *threads = (cthread_t *)AllocFn(threadsSize);

    if (threads == NULL)
    {
        K_LOG_ERROR("Failed to allocate memory for thread handles.");
        DeAllocFn(args, argsSize);
        return false;
    }

    size_t threadArgsSize = sizeof(cthread_args) * threadCount;
    cthread_args *threadArgs = (cthread_args *)AllocFn(threadArgsSize);

    if (threadArgs == NULL)
    {
        K_LOG_ERROR("Failed to allocate memory for thread arguments.");
        DeAllocFn(threads, threadsSize);
        DeAllocFn(args, argsSize);
        return false;
    }

    k_atomic_bool errorFlag = false;

    // Calculate chunk sizes for even distribution, remainder elements are distributed among first threads
    size_t baseChunkSize = arraySize / threadCount;
    size_t remainder = arraySize % threadCount;

    for (int t = 0; t < threadCount; ++t)
    {
        // Calculate the number of elements for this thread to process
        size_t chunkSize = baseChunkSize + (t < remainder ? 1 : 0);

        // Calculate pointer to start of this thread's data chunk
        args[t].startPoint = (char *)array + (t * baseChunkSize + (t < remainder ? t : remainder)) * elementSize;

        args[t].processCount = chunkSize;
        args[t].elementSize = elementSize;
        args[t].pbSpaces = pbSpacesStoreArray;
        args[t].pbSpaceCount = pbSpaceCount;

        args[t].indexerFn = indexerFn;
        args[t].indexerFnData = indexerFnData;
        args[t].errorFlag = &errorFlag;

        // Reset error flag before starting each thread
        ATOMIC_STORE_BOOL(&errorFlag, false);

        // Create thread and handle possible failure
        if (cthreads_thread_create(&threads[t], NULL, thread_counting, &args[t], &threadArgs[t]) != 0)
        {
            K_LOG_ERROR("Failed to create thread %d.", t);

            // Cancel already running threads on failure to create new thread
            for (int j = 0; j < t; ++j)
            {
                cthreads_thread_cancel(threads[j]);
            }

            DeAllocFn(threadArgs, threadArgsSize);
            DeAllocFn(threads, threadsSize);
            DeAllocFn(args, argsSize);

            return false;
        }
    }

    // Wait for all threads to finish processing
    for (int t = 0; t < threadCount; ++t)
    {
        int joinResult = cthreads_thread_timedjoin(threads[t], threadTimeoutMs);

        // Handle thread join failure
        if (joinResult != 1)
        {
            K_LOG_ERROR("Failed to join thread %d.", t);

            // Cancel remaining threads which have not finished
            for (int j = t + 1; j < threadCount; ++j)
            {
                cthreads_thread_cancel(threads[j]);
            }

            DeAllocFn(threadArgs, threadArgsSize);
            DeAllocFn(threads, threadsSize);
            DeAllocFn(args, argsSize);

            return false;
        }
    }

    DeAllocFn(threadArgs, threadArgsSize);
    DeAllocFn(threads, threadsSize);
    DeAllocFn(args, argsSize);

    // Check shared error flag for any out-of-bounds index detected during processing
    if (ATOMIC_LOAD_BOOL(&errorFlag))
    {
        K_LOG_ERROR("Indexer function returned an out-of-bounds index.");
        return false;
    }

    return true;
}

static void *thread_copying(void *arg)
{
    CopyThreadArgs *args = (CopyThreadArgs *)arg;
    for (size_t i = 0; i < args->processCount; ++i)
    {
        void *element = GetIndex(args->startPoint, i, args->elementSize);
        size_t index = args->indexerFn(element, args->indexerFnData);

        if (index >= args->pbSpaceCount)
        {
            K_LOG_ERROR("Thread %lu: Indexer function returned out-of-bounds index: " SIZE_T_IDENTIFIER, cthreads_thread_id(cthreads_thread_self()), index);
            ATOMIC_STORE_BOOL(args->errorFlag, true);
            return (void *)-1; // Indicate error
        }

        PossibilitySpace *space = &args->pbSpaces[index];
        size_t pos = ATOMIC_ADD_SIZE_T(&space->Size, 1);

        // For debugging purposes
        if (pos > space->Capacity)
        {
            K_LOG_ERROR("Thread %lu: Target space index " SIZE_T_IDENTIFIER " is out of bounds. Size: " SIZE_T_IDENTIFIER ", Pos: " SIZE_T_IDENTIFIER ", Capacity: " SIZE_T_IDENTIFIER, cthreads_thread_id(cthreads_thread_self()), index, space->Size, pos, space->Capacity);
            ATOMIC_STORE_BOOL(args->errorFlag, true);
            return (void *)-1; // Indicate error
        }

        // For debugging purposes
        if (pos == 0)
        {
            K_LOG_ERROR("Thread %lu: Target space index " SIZE_T_IDENTIFIER " has zero size. Size: " SIZE_T_IDENTIFIER ", Pos: " SIZE_T_IDENTIFIER ", Capacity: " SIZE_T_IDENTIFIER, cthreads_thread_id(cthreads_thread_self()), index, space->Size, pos, space->Capacity);
            ATOMIC_STORE_BOOL(args->errorFlag, true);
            return (void *)-1; // Indicate error
        }

        memcpy(GetIndex(args->pbSpaces[index].Element, pos - 1, args->elementSize), element, args->elementSize);
    }
    return (void *)0;
}

bool CopyElementsToPossibilitySpaces_Parallel(
    void *array,
    size_t arraySize,
    size_t elementSize,
    size_t pbSpaceCount,
    PossibilitySpace *pbSpacesStoreArray,
    SpaceIndexerFn indexerFn,
    void *indexerFnData,
    size_t threadCount,
    unsigned long threadTimeoutMs)
{
    // --- Input validation starts here ---
    if (array == NULL)
    {
        K_LOG_ERROR("Input 'array' pointer is NULL.");
        return false;
    }

    if (arraySize == 0)
    {
        K_LOG_ERROR("Input 'arraySize' is zero.");
        return false;
    }

    if (elementSize == 0)
    {
        K_LOG_ERROR("Input 'elementSize' is zero.");
        return false;
    }

    if (pbSpaceCount == 0)
    {
        K_LOG_ERROR("Input 'pbSpaceCount' is zero.");
        return false;
    }

    if (pbSpacesStoreArray == NULL)
    {
        K_LOG_ERROR("Input 'pbSpacesStoreArray' pointer is NULL.");
        return false;
    }

    if (indexerFn == NULL)
    {
        K_LOG_ERROR("Input 'indexerFn' pointer is NULL.");
        return false;
    }

    if (threadCount == 0)
    {
        K_LOG_ERROR("Input 'threadCount' is zero.");
        return false;
    }

    if (threadTimeoutMs == 0)
    {
        K_LOG_ERROR("Input 'threadTimeoutMs' is zero.");
        return false;
    }
    // --- Input validation ends here ---

    /*
    // Initialize counts array atomically to zero before starting threads
    for (size_t i = 0; i < pbSpaceCount; ++i)
    {
        ATOMIC_STORE_SIZE_T(&counts[i], 0);
    }
    */

    size_t argsSize = sizeof(CopyThreadArgs) * threadCount;
    CopyThreadArgs *args = (CopyThreadArgs *)AllocFn(argsSize);

    if (args == NULL)
    {
        K_LOG_ERROR("Failed to allocate memory for thread arguments.");
        return false;
    }

    size_t threadsSize = sizeof(cthread_t) * threadCount;
    cthread_t *threads = (cthread_t *)AllocFn(threadsSize);

    if (threads == NULL)
    {
        K_LOG_ERROR("Failed to allocate memory for thread handles.");
        DeAllocFn(args, argsSize);
        return false;
    }

    size_t threadArgsSize = sizeof(cthread_args) * threadCount;
    cthread_args *threadArgs = (cthread_args *)AllocFn(threadArgsSize);

    if (threadArgs == NULL)
    {
        K_LOG_ERROR("Failed to allocate memory for thread arguments.");
        DeAllocFn(threads, threadsSize);
        DeAllocFn(args, argsSize);
        return false;
    }

    k_atomic_bool errorFlag = false;

    // Calculate chunk sizes for even distribution, remainder elements are distributed among first threads
    size_t baseChunkSize = arraySize / threadCount;
    size_t remainder = arraySize % threadCount;

    for (int t = 0; t < threadCount; ++t)
    {
        // Calculate the number of elements for this thread to process
        size_t chunkSize = baseChunkSize + (t < remainder ? 1 : 0);

        // Calculate pointer to start of this thread's data chunk
        args[t].startPoint = (char *)array + (t * baseChunkSize + (t < remainder ? t : remainder)) * elementSize;

        args[t].processCount = chunkSize;
        args[t].elementSize = elementSize;
        args[t].pbSpaces = pbSpacesStoreArray;
        args[t].pbSpaceCount = pbSpaceCount;

        args[t].indexerFn = indexerFn;
        args[t].indexerFnData = indexerFnData;
        args[t].errorFlag = &errorFlag;

        // Reset error flag before starting each thread
        ATOMIC_STORE_BOOL(&errorFlag, false);

        // Create thread and handle possible failure
        if (cthreads_thread_create(&threads[t], NULL, thread_copying, &args[t], &threadArgs[t]) != 0)
        {
            K_LOG_ERROR("Failed to create thread %d.", t);

            // Cancel already running threads on failure to create new thread
            for (int j = 0; j < t; ++j)
            {
                cthreads_thread_cancel(threads[j]);
            }

            DeAllocFn(threadArgs, threadArgsSize);
            DeAllocFn(threads, threadsSize);
            DeAllocFn(args, argsSize);

            return false;
        }
    }

    // Wait for all threads to finish processing
    for (int t = 0; t < threadCount; ++t)
    {
        int joinResult = cthreads_thread_timedjoin(threads[t], threadTimeoutMs);

        // Handle thread join failure
        if (joinResult != 1)
        {
            K_LOG_ERROR("Failed to join thread %d.", t);

            // Cancel remaining threads which have not finished
            for (int j = t + 1; j < threadCount; ++j)
            {
                cthreads_thread_cancel(threads[j]);
            }

            DeAllocFn(threadArgs, threadArgsSize);
            DeAllocFn(threads, threadsSize);
            DeAllocFn(args, argsSize);

            return false;
        }
    }

    DeAllocFn(threadArgs, threadArgsSize);
    DeAllocFn(threads, threadsSize);
    DeAllocFn(args, argsSize);

    // Check shared error flag for any out-of-bounds index detected during processing
    if (ATOMIC_LOAD_BOOL(&errorFlag))
    {
        K_LOG_ERROR("Indexer function returned an out-of-bounds index.");
        return false;
    }

    return true;
}

static void *thread_sorting(void *arg)
{
    if (arg == NULL)
    {
        K_LOG_ERROR("Thread argument is NULL.");
        return (void *)-1; // Indicate error
    }

    SortThreadArgs *args = (SortThreadArgs *)arg;

    K_LOG_DEBUG("Thread %lu: Starting sorting for " SIZE_T_IDENTIFIER " PossibilitySpaces", cthreads_thread_id(cthreads_thread_self()), args->pbSpaceCount);

    if (args->pbSpaces == NULL)
    {
        K_LOG_ERROR("Thread %lu: pbSpaces is NULL", cthreads_thread_id(cthreads_thread_self()));
        ATOMIC_STORE_BOOL(args->errorFlag, true);
        return (void *)-1;
    }

    if(args->pbSpaceCount == 0)
    {
        K_LOG_DEBUG("Thread %lu: pbSpaceCount is zero", cthreads_thread_id(cthreads_thread_self()));
    }

    if(args->elementSize == 0)
    {
        K_LOG_ERROR("Thread %lu: elementSize is zero", cthreads_thread_id(cthreads_thread_self()));
        ATOMIC_STORE_BOOL(args->errorFlag, true);
        return (void *)-1;
    }

    if (args->sortFn == NULL)
    {
        K_LOG_ERROR("Thread %lu: sortFn is NULL", cthreads_thread_id(cthreads_thread_self()));
        ATOMIC_STORE_BOOL(args->errorFlag, true);
        return (void *)-1;
    }

    if (args->comparerFn == NULL)
    {
        K_LOG_ERROR("Thread %lu: comparerFn is NULL", cthreads_thread_id(cthreads_thread_self()));
        ATOMIC_STORE_BOOL(args->errorFlag, true);
        return (void *)-1;
    }

    for (size_t i = 0; i < args->pbSpaceCount; i++)
    {
        if (args->pbSpaces[i].Size == 0)
        {
            K_LOG_DEBUG("Thread %lu: Invalid PossibilitySpace at index " SIZE_T_IDENTIFIER ". Size is zero", cthreads_thread_id(cthreads_thread_self()), i);
            continue;
        }

        if (args->pbSpaces[i].Element == NULL)
        {
            K_LOG_ERROR("Thread %lu: Invalid PossibilitySpace at index " SIZE_T_IDENTIFIER ". Element is NULL", cthreads_thread_id(cthreads_thread_self()), i);
            ATOMIC_STORE_BOOL(args->errorFlag, true);
            return (void *)-1; // Indicate error
        }

        if (args->pbSpaces[i].Size == 1)
        {
            K_LOG_DEBUG("Thread %lu: Skipping sorting for PossibilitySpace at index " SIZE_T_IDENTIFIER " with size 1", cthreads_thread_id(cthreads_thread_self()), i);
            continue; // No need to sort if size is 1
        }

        // For debugging purposes
        if (args->pbSpaces[i].Size > args->pbSpaces[i].Capacity)
        {
            K_LOG_ERROR("Thread %lu: Invalid PossibilitySpace at index " SIZE_T_IDENTIFIER ". Size " SIZE_T_IDENTIFIER " exceeds Capacity " SIZE_T_IDENTIFIER, cthreads_thread_id(cthreads_thread_self()), i, args->pbSpaces[i].Size, args->pbSpaces[i].Capacity);
            ATOMIC_STORE_BOOL(args->errorFlag, true);
            return (void *)-1; // Indicate error
        }

        args->sortFn(args->pbSpaces[i].Element, args->pbSpaces[i].Size, args->elementSize, args->comparerFn);
    }

    K_LOG_DEBUG("Thread %lu: Sorting completed for " SIZE_T_IDENTIFIER " PossibilitySpaces", cthreads_thread_id(cthreads_thread_self()), args->pbSpaceCount);

    return (void *)0;
}

bool SortPossibilitySpaces_Parallel(
    size_t elementSize,
    PossibilitySpace *pbSpaces,
    size_t pbSpaceCount,
    SortFunction sortFn,
    CompareFunction comparerFn,
    size_t threadCount,
    unsigned long threadTimeoutMs)
{
    // --- Input validation starts here ---
    if (elementSize == 0)
    {
        K_LOG_ERROR("Input 'elementSize' is zero.");
        return false;
    }

    if (pbSpaces == NULL)
    {
        K_LOG_ERROR("Input 'pbSpaces' pointer is NULL.");
        return false;
    }

    if (pbSpaceCount == 0)
    {
        K_LOG_ERROR("Input 'pbSpaceCount' is zero.");
        return false;
    }

    if (sortFn == NULL)
    {
        K_LOG_ERROR("Input 'sortFn' pointer is NULL.");
        return false;
    }

    if (comparerFn == NULL)
    {
        K_LOG_ERROR("Input 'comparerFn' pointer is NULL.");
        return false;
    }

    if (threadCount == 0)
    {
        K_LOG_ERROR("Input 'threadCount' is zero.");
        return false;
    }

    if (threadTimeoutMs == 0)
    {
        K_LOG_ERROR("Input 'threadTimeoutMs' is zero.");
        return false;
    }
    // --- Input validation ends here ---

    K_LOG_DEBUG("Starting SortPossibilitySpaces_Parallel...");

    size_t argsSize = sizeof(SortThreadArgs) * threadCount;
    SortThreadArgs *args = (SortThreadArgs *)AllocFn(argsSize);
    if (!args)
    {
        K_LOG_ERROR("Failed to allocate memory for thread arguments.");
        return false;
    }

    size_t threadsSize = sizeof(cthread_t) * threadCount;
    cthread_t *threads = (cthread_t *)AllocFn(threadsSize);
    if (!threads)
    {
        K_LOG_ERROR("Failed to allocate memory for thread handles.");
        DeAllocFn(args, argsSize);
        return false;
    }

    size_t threadArgsSize = sizeof(cthread_args) * threadCount;
    cthread_args *threadArgs = (cthread_args *)AllocFn(threadArgsSize);
    if (!threadArgs)
    {
        K_LOG_ERROR("Failed to allocate memory for thread arguments.");
        DeAllocFn(threads, threadsSize);
        DeAllocFn(args, argsSize);
        return false;
    }

    k_atomic_bool errorFlag = false;

    ATOMIC_STORE_BOOL(&errorFlag, false);

    size_t baseChunkSize = pbSpaceCount / threadCount;
    size_t remainder = pbSpaceCount % threadCount;

    for (int t = 0; t < threadCount; ++t)
    {
        size_t chunkCount = baseChunkSize + (t < remainder ? 1 : 0);
        size_t startIndex = t * baseChunkSize + (t < remainder ? t : remainder);

        K_LOG_DEBUG("Space Count: " SIZE_T_IDENTIFIER ", Base Chunk Size: " SIZE_T_IDENTIFIER ", Remainder: " SIZE_T_IDENTIFIER ", Chunk Count: " SIZE_T_IDENTIFIER ", Start Index: " SIZE_T_IDENTIFIER, pbSpaceCount, baseChunkSize, remainder, chunkCount, startIndex);

        args[t].pbSpaces = &pbSpaces[startIndex];
        args[t].pbSpaceCount = chunkCount;
        args[t].elementSize = elementSize;
        args[t].sortFn = sortFn;
        args[t].comparerFn = comparerFn;
        args[t].errorFlag = &errorFlag;

        //ATOMIC_STORE_BOOL(&errorFlag, false);

        if (cthreads_thread_create(&threads[t], NULL, thread_sorting, &args[t], &threadArgs[t]) != 0)
        {
            K_LOG_ERROR("Failed to create thread %d.", t);
            for (int j = 0; j < t; ++j)
            {
                cthreads_thread_cancel(threads[j]);
            }

            DeAllocFn(threadArgs, threadArgsSize);
            DeAllocFn(threads, threadsSize);
            DeAllocFn(args, argsSize);
            return false;
        }
    }

    K_LOG_DEBUG("All threads created successfully. Waiting for them to finish...");

    for (int t = 0; t < threadCount; ++t)
    {
        int joinResult = cthreads_thread_timedjoin(threads[t], threadTimeoutMs);
        if (joinResult != 1)
        {
            K_LOG_ERROR("Failed to join thread %d.", t);
            for (int j = t + 1; j < threadCount; ++j)
            {
                cthreads_thread_cancel(threads[j]);
            }

            DeAllocFn(threadArgs, threadArgsSize);
            DeAllocFn(threads, threadsSize);
            DeAllocFn(args, argsSize);
            return false;
        }
    }

    K_LOG_DEBUG("All threads joined successfully.");

    DeAllocFn(threadArgs, threadArgsSize);
    DeAllocFn(threads, threadsSize);
    DeAllocFn(args, argsSize);

    if (ATOMIC_LOAD_BOOL(&errorFlag))
    {
        K_LOG_ERROR("Error occurred during sorting threads.");
        return false;
    }

    K_LOG_DEBUG("SortPossibilitySpaces_Parallel completed successfully.");

    return true;
}

PossibilitySpace *SortV2_Parallel(void *array, size_t arraySize, size_t elementSize, size_t pbSpaceCount, SpaceIndexerFn indexerFn, void *indexerFnData, CompareFunction comparerFn, SortFunction sortFn, size_t threadCount, unsigned long threadTimeoutMs)
{
    K_LOG_DEBUG("Starting SortV2...");

    PossibilitySpace *pbSpaces = CreatePossibilitySpaceArray(pbSpaceCount);

    if (pbSpaces == NULL)
    {
        K_LOG_ERROR("Failed to create PossibilitySpace array.");
        return NULL;
    }

    K_LOG_DEBUG("Counting possibility spaces...");
    if (!CountPossibilitySpaces_Parallel(array, arraySize, elementSize, pbSpaceCount, pbSpaces, indexerFn, indexerFnData, threadCount, threadTimeoutMs))
    {
        K_LOG_ERROR("Failed to count possibility spaces.");
        FreePossibilitySpaceArray(pbSpaces, pbSpaceCount, elementSize);
        return NULL;
    }

    #ifdef DEBUG
    for(size_t i = 0; i < pbSpaceCount; ++i)
    {
        K_LOG_DEBUG("PossibilitySpace[" SIZE_T_IDENTIFIER "]: Size = " SIZE_T_IDENTIFIER ", Capacity = " SIZE_T_IDENTIFIER, i, pbSpaces[i].Size, pbSpaces[i].Capacity);
    }
    #endif

    K_LOG_DEBUG("Allocating elements for possibility spaces...");
    if (!AllocatePossibilitySpaceElements(elementSize, pbSpaceCount, pbSpaces))
    {
        K_LOG_ERROR("Failed to allocate elements for possibility spaces.");
        FreePossibilitySpaceArray(pbSpaces, pbSpaceCount, elementSize);
        return NULL;
    }

    #ifdef DEBUG
    for(size_t i = 0; i < pbSpaceCount; ++i)
    {
        K_LOG_DEBUG("PossibilitySpace[" SIZE_T_IDENTIFIER "]: Size = " SIZE_T_IDENTIFIER ", Capacity = " SIZE_T_IDENTIFIER, i, pbSpaces[i].Size, pbSpaces[i].Capacity);
    }
    #endif

    K_LOG_DEBUG("Copying elements to possibility spaces...");
    if (!CopyElementsToPossibilitySpaces_Parallel(array, arraySize, elementSize, pbSpaceCount, pbSpaces, indexerFn, indexerFnData, threadCount, threadTimeoutMs))
    {
        K_LOG_ERROR("Failed to copy elements to possibility spaces.");
        FreePossibilitySpaceArray(pbSpaces, pbSpaceCount, elementSize);
        return NULL;
    }

    #ifdef DEBUG
    for(size_t i = 0; i < pbSpaceCount; ++i)
    {
        K_LOG_DEBUG("PossibilitySpace[" SIZE_T_IDENTIFIER "]: Size = " SIZE_T_IDENTIFIER ", Capacity = " SIZE_T_IDENTIFIER, i, pbSpaces[i].Size, pbSpaces[i].Capacity);
    }
    #endif

    K_LOG_DEBUG("Sorting possibility spaces...");
    if (!SortPossibilitySpaces_Parallel(elementSize, pbSpaces, pbSpaceCount, sortFn, comparerFn, threadCount, threadTimeoutMs))
    {
        K_LOG_ERROR("Failed to sort possibility spaces.");
        FreePossibilitySpaceArray(pbSpaces, pbSpaceCount, elementSize);
        return NULL;
    }

    K_LOG_DEBUG("Sorting completed successfully.");
    return pbSpaces;
}