#include "KOKESortV2Parallel.h"

/**
 * @brief Thread worker function: process a subset of elements to count occurrences in index buckets.
 * 
 * @param arg Pointer to ThreadArgs struct
 * @return int Returns 0 on success, -1 on error (out-of-bound index)
 */
static void* thread_counting(void *arg)
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
            ATOMIC_STORE_SIZE_T(args->errorFlag, true);
            return -1;
        }

        // Atomically increment the count for this index
        ATOMIC_ADD_SIZE_T(&args->pbSpaces[index].Capacity, 1);
    }

    return 0;
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
    PossibilitySpace* pbSpacesStoreArray,
    SpaceIndexerFn indexerFn,
    size_t threadCount,
    unsigned long threadTimeoutMs
)
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

    // --- Input validation ends here ---

    /*
    // Initialize counts array atomically to zero before starting threads
    for (size_t i = 0; i < pbSpaceCount; ++i)
    {
        ATOMIC_STORE_SIZE_T(&counts[i], 0);
    }
    */

    size_t argsSize = sizeof(CountThreadArgs) * threadCount;
    CountThreadArgs* args = (CountThreadArgs*)AllocFn(argsSize);

    if(args == NULL)
    {
        K_LOG_ERROR("Failed to allocate memory for thread arguments.");
        return false;
    }

    size_t threadsSize = sizeof(cthread_t) * threadCount;
    cthread_t *threads = (cthread_t*)AllocFn(threadsSize);

    if(threads == NULL)
    {
        K_LOG_ERROR("Failed to allocate memory for thread handles.");
        DeAllocFn(args, argsSize);
        return false;
    }

    size_t threadArgsSize = sizeof(cthread_args) * threadCount;
    cthread_args* threadArgs = (cthread_args*)AllocFn(threadArgsSize);

    if(threadArgs == NULL)
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
        args[t].errorFlag = &errorFlag;

        // Reset error flag before starting each thread
        ATOMIC_STORE_SIZE_T(&errorFlag, false);

        // Create thread and handle possible failure
        if (cthreads_thread_create(&threads[t], NULL, thread_counting, &args[t], &threadArgs[t]) != 0)
        {
            K_LOG_ERROR("Failed to create thread %d.", t);

            // Cancel already running threads on failure to create new thread
            for (int j = 0; j < t; ++j)
            {
                cthreads_thread_cancel(threads[j]);
            }

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

            return false;
        }
    }

    // Check shared error flag for any out-of-bounds index detected during processing
    if (ATOMIC_LOAD_SIZE_T(&errorFlag))
    {
        K_LOG_ERROR("Indexer function returned an out-of-bounds index.");
        return false;
    }

    return true;
}

void* thread_copying(void* arg) 
{
    //WARNING: IndexerFn Data can be accour Race Condition, it's not wrapped yet.

    CopyThreadArgs* args = (CopyThreadArgs*)arg;
    for (size_t i = args->start; i < args->end; ++i) 
    {
        void* element = (char*)args->array + i * args->elementSize;
        size_t index = args->indexerFn(element, args->indexerFnData);
        if (index < args->pbSpaceCount) 
        {
            PossibilitySpace* space = &args->spaces[index];
            size_t pos = atomic_fetch_add(&space->Size, 1);
            memcpy((char*)space->Element + pos * args->elementSize, element, args->elementSize);
        }
    }
    return NULL;
}

void ParallelCopy(
    void* array, 
    size_t arraySize, 
    size_t elementSize,
    size_t pbSpaceCount, 
    PossibilitySpace* pbSpacesStoreArray,
    SpaceIndexerFn indexerFn,
    size_t threadCount,
    unsigned long threadTimeoutMs
) {
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

    // --- Input validation ends here ---

    /*
    // Initialize counts array atomically to zero before starting threads
    for (size_t i = 0; i < pbSpaceCount; ++i)
    {
        ATOMIC_STORE_SIZE_T(&counts[i], 0);
    }
    */

    size_t argsSize = sizeof(CountThreadArgs) * threadCount;
    CountThreadArgs* args = (CountThreadArgs*)AllocFn(argsSize);

    if(args == NULL)
    {
        K_LOG_ERROR("Failed to allocate memory for thread arguments.");
        return false;
    }

    size_t threadsSize = sizeof(cthread_t) * threadCount;
    cthread_t *threads = (cthread_t*)AllocFn(threadsSize);

    if(threads == NULL)
    {
        K_LOG_ERROR("Failed to allocate memory for thread handles.");
        DeAllocFn(args, argsSize);
        return false;
    }

    size_t threadArgsSize = sizeof(cthread_args) * threadCount;
    cthread_args* threadArgs = (cthread_args*)AllocFn(threadArgsSize);

    if(threadArgs == NULL)
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
        args[t].errorFlag = &errorFlag;

        // Reset error flag before starting each thread
        ATOMIC_STORE_SIZE_T(&errorFlag, false);

        // Create thread and handle possible failure
        if (cthreads_thread_create(&threads[t], NULL, thread_counting, &args[t], &threadArgs[t]) != 0)
        {
            K_LOG_ERROR("Failed to create thread %d.", t);

            // Cancel already running threads on failure to create new thread
            for (int j = 0; j < t; ++j)
            {
                cthreads_thread_cancel(threads[j]);
            }

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

            return false;
        }
    }

    // Check shared error flag for any out-of-bounds index detected during processing
    if (ATOMIC_LOAD_SIZE_T(&errorFlag))
    {
        K_LOG_ERROR("Indexer function returned an out-of-bounds index.");
        return false;
    }

    return true;
}

PossibilitySpace* SortV2(void *array, size_t arraySize, size_t elementSize, size_t pbSpaceCount, SpaceIndexerFn indexerFn, void* indexerFnData, CompareFunction comparerFn, SortFunction sortFn, size_t threadCount, unsigned long threadTimeoutMs)
{
    K_LOG_DEBUG("Starting SortV2...");

    PossibilitySpace* pbSpaces = CreatePossibilitySpaceArray(pbSpaceCount);

    if (pbSpaces == NULL)
    {
        K_LOG_ERROR("Failed to create PossibilitySpace array.");
        return NULL;
    }

    K_LOG_DEBUG("Counting possibility spaces...");
    if (!CountPossibilitySpaces_Parallel(array, arraySize, elementSize, pbSpaces, pbSpaceCount, indexerFn, indexerFnData, threadCount, threadTimeoutMs))
    {
        K_LOG_ERROR("Failed to count possibility spaces.");
        FreePossibilitySpaceArray(&pbSpaces, pbSpaceCount, elementSize);
        return NULL;
    }

    K_LOG_DEBUG("Allocating elements for possibility spaces...");
    if (!AllocatePossibilitySpaceElements(elementSize, pbSpaceCount, pbSpaces))
    {
        K_LOG_ERROR("Failed to allocate elements for possibility spaces.");
        FreePossibilitySpaceArray(&pbSpaces, pbSpaceCount, elementSize);
        return NULL;
    }

    K_LOG_DEBUG("Copying elements to possibility spaces...");
    if (!CopyElementsToPossibilitySpaces(array, arraySize, elementSize, pbSpaceCount, pbSpaces, indexerFn, indexerFnData))
    {
        K_LOG_ERROR("Failed to copy elements to possibility spaces.");
        FreePossibilitySpaceArray(&pbSpaces, pbSpaceCount, elementSize);
        return NULL;
    }

    K_LOG_DEBUG("Sorting possibility spaces...");
    if (!SortPossibilitySpaces(pbSpaceCount, pbSpaces, elementSize, comparerFn, sortFn))
    {
        K_LOG_ERROR("Failed to sort possibility spaces.");
        FreePossibilitySpaceArray(&pbSpaces, pbSpaceCount, elementSize);
        return NULL;
    }

    K_LOG_DEBUG("Sorting completed successfully.");
    return pbSpaces;
}