const { Array1DTracer, LogTracer, Layout, VerticalLayout, Tracer } = require('algorithm-visualizer');

function generateRandomArray(size, min, max) {
  const uniqueNumbers = [];
  
  // Create a set of unique numbers within the range
  for (let i = min; i <= max; i++) {
    uniqueNumbers.push(i);
  }
  
  // Shuffle the array to ensure randomness
  for (let i = uniqueNumbers.length - 1; i > 0; i--) {
    const j = Math.floor(Math.random() * (i + 1));
    [uniqueNumbers[i], uniqueNumbers[j]] = [uniqueNumbers[j], uniqueNumbers[i]]; // Swap elements
  }
  
  // Return a subarray of the desired size
  return uniqueNumbers.slice(0, size);
}

const array = generateRandomArray(10, 1, 50);

// Layout Tracers
const log = new LogTracer('Log');
const tracerInPlace = new Array1DTracer('In-Place KOKE Sort');
const tracerOriginal = new Array1DTracer('Original Array (Read Only)');
const tracerBuffer = new Array1DTracer('Buffer Array (Non-In-Place KOKE Sort Output)');

Layout.setRoot(new VerticalLayout([
  tracerInPlace,
  new VerticalLayout([tracerOriginal, tracerBuffer]),
  log
]));

tracerInPlace.set(array.slice());
tracerOriginal.set(array);
tracerBuffer.set([]);

Tracer.delay();

// In-Place KOKE Sort Function
function inPlaceKokeSort(arr, tracer, log, callback) {
  log.println('🔁 Starting In-Place KOKE Sort');
  let i = 1;
  let shiftCount = 0;
  let accessCount = 0;

  while (i < arr.length) {
    const key = arr[i];
    accessCount++;

    let left = 0;
    let right = i - 1;

    // Perform binary search
    while (left <= right) {
      const mid = Math.floor((left + right) / 2);
      tracer.select(mid);
      Tracer.delay();

      accessCount++;
      if (arr[mid] < key) left = mid + 1;
      else right = mid - 1;
      tracer.deselect(mid);
    }

    let j = i;
    // Perform shifts to insert the element
    while (j > left) {
      arr[j] = arr[j - 1];
      tracer.patch(j, arr[j]);
      tracer.select(j);
      shiftCount++;
      log.println(`Shifted: ${arr[j]} to index ${j}`);
      Tracer.delay();
      tracer.deselect(j);
      j--;
    }

    arr[left] = key;
    tracer.patch(left, key);
    tracer.select(left);
    log.println(`Inserted ${key} at index ${left}`);
    Tracer.delay();
    tracer.deselect(left);

    i++;
  }

  log.println(`✅ In-Place KOKE Sort Completed (Total Shifts: ${shiftCount}, Total Accesses: ${accessCount})`);
  callback();
}

// Non-In-Place KOKE Sort Function
function nonInPlaceKokeSort(input, originalTracer, bufferTracer, log) {
  log.println('\n🔁 Starting Non-In-Place KOKE Sort');
  const result = [];
  let i = 0;
  let shiftCount = 0;
  let accessCount = 0;

  while (i < input.length) {
    const val = input[i];
    accessCount++;

    originalTracer.select(i);
    log.println(`Reading ${val} from index ${i}`);
    Tracer.delay();
    originalTracer.deselect(i);

    let left = 0;
    let right = result.length - 1;

    // Perform binary insertion
    while (left <= right) {
      const mid = Math.floor((left + right) / 2);
      bufferTracer.select(mid);
      Tracer.delay();

      accessCount++;
      if (result[mid] < val) left = mid + 1;
      else right = mid - 1;
      bufferTracer.deselect(mid);
    }

    shiftCount += result.length - left;
    result.splice(left, 0, val);
    bufferTracer.set(result);
    bufferTracer.select(left);
    log.println(`Inserted ${val} at buffer index ${left} (Shifts: ${result.length - left - 1})`);
    Tracer.delay();
    bufferTracer.deselect(left);

    i++;
  }

  log.println(`✅ Non-In-Place KOKE Sort Completed (Total Shifts: ${shiftCount}, Total Accesses: ${accessCount})`);
}

// Run sequentially without async/await
inPlaceKokeSort(array.slice(), tracerInPlace, log, () => {
  nonInPlaceKokeSort(array.slice(), tracerOriginal, tracerBuffer, log);
});
