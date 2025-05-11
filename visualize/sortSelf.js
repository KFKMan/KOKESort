//It's using https://sortvisualizer.com/ Api

function getInsertIndexBS(sortedArray, arraySize, element) {
    let left = 0;
    let right = arraySize;
    while (left < right) {
        let mid = left + ((right - left) / 2 | 0);
        if (getValue(mid) < element) {
            left = mid + 1;
        }
        else {
            right = mid;
        }
    }
    return left;
}

async function addElementToSortedArray(sortedArray, arraySize, element, elementIndex) {
    let insertIndex = getInsertIndexBS(sortedArray, arraySize, element);
    let currentIndex = arraySize;
    while (currentIndex > insertIndex) {
        await swap(currentIndex, currentIndex - 1);
        currentIndex--;
    }
    //await swap(insertIndex, elementIndex);
}

async function KOKESort(array)
{
    let size = array.length;
    
    for (let i = 1; i < size; i++) {
        let element = getValue(i);
        await addElementToSortedArray(array, i, element, i);
    }
}
