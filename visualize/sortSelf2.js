//It's using https://algorithm-visualizer.org/ Api

// import visualization libraries {
const { Tracer, Array1DTracer, ChartTracer, LogTracer, Randomize, Layout, VerticalLayout } = require('algorithm-visualizer');
// }

// define tracer variables {
const chart = new ChartTracer();
const tracer = new Array1DTracer();
const logger = new LogTracer();
Layout.setRoot(new VerticalLayout([chart, tracer, logger]));
const D = Randomize.Array1D({ N: 15, value: () => Randomize.Integer({ min: 0, max: 1000 }), sorted: true });
tracer.set(D);
tracer.chart(chart);
Tracer.delay();
// }

function BinarySearch(array, element) { // array = sorted array, element = element to be found
    let minIndex = 0;
    let maxIndex = array.length - 1;
    let testElement;

    while (minIndex <= maxIndex) {
        const middleIndex = Math.floor((minIndex + maxIndex) / 2);
        testElement = array[middleIndex];

        // visualize {
        tracer.select(minIndex, maxIndex);
        Tracer.delay();
        tracer.patch(middleIndex);
        logger.println(`Searching at index: ${middleIndex}`);
        Tracer.delay();
        tracer.depatch(middleIndex);
        tracer.deselect(minIndex, maxIndex);
        // }

        if (testElement < element) {
            // logger {
            logger.println('Going right.');
            // }
            minIndex = middleIndex + 1;
        } else if (testElement > element) {
            // logger {
            logger.println('Going left.');
            // }
            maxIndex = middleIndex - 1;
        } else {
            // visualize {
            logger.println(`${element} is found at position ${middleIndex}!`);
            tracer.select(middleIndex);
            // }

            return middleIndex;
        }
    }

    // logger {
    logger.println(`${element} is not found!`);
    // }
    return -1;
}

function getInsertIndexBS(sortedArray, arraySize, element) {
    let left = 0;
    let right = arraySize;
    while (left < right) {
        let mid = left + ((right - left) / 2 | 0);

        tracer.select(left, right);
        Tracer.delay();
        tracer.patch(mid);
        Tracer.delay();
        tracer.depatch(mid);
        tracer.deselect(left, right);

        if (sortedArray[mid] < element) {
            left = mid + 1;
        }
        else {
            right = mid;
        }
    }
    return left;
}

function addElementToSortedArray(sortedArray, arraySize, element, elementIndex) {
    let insertIndex = getInsertIndexBS(sortedArray, arraySize, element);
    let currentIndex = arraySize;

    tracer.deselect(elementIndex);
    while (currentIndex > insertIndex) {
        tracer.patch(currentIndex, currentIndex - 1);
        Tracer.delay();
        tracer.depatch(currentIndex, currentIndex - 1);

        sortedArray[currentIndex] = sortedArray[currentIndex - 1];
        currentIndex--;
    }
}

function KOKESort(array) {
    let size = array.length;

    for (let i = 1; i < size; i++) 
    {
        tracer.select(i);
        let element = getValue(i);
        addElementToSortedArray(array, i, element, i);
    }
}

KOKESort(D);