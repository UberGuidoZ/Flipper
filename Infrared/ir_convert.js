//usage: node irconvert.js <pronto hex>
//steps from https://stackoverflow.com/a/27323452
//sourced from https://gist.github.com/XMB5/a877ab620d812260f2da8380aac050d3

numbers=process.argv[2].split(' ').map(x=>parseInt(x,16))
fullSequenceConverted=[]

carrierFrequency = 1000000/(numbers[1] * .241246)
codeLength = numbers[2]
repeatCodeLength = numbers[3]

for(i=4;i<numbers.length;i++) {
    convertedToMicrosec = Math.round(1000000*(numbers[i]/carrierFrequency))
    fullSequenceConverted.push(convertedToMicrosec)
}

sequence1EndPoint = 2 * codeLength
sequence2EndPoint = sequence1EndPoint + 2 * repeatCodeLength

firstSequence = fullSequenceConverted.slice(0, sequence1EndPoint)
secondSequence = fullSequenceConverted.slice(sequence1EndPoint, sequence2EndPoint)

console.log('first: ' + firstSequence.join(', '))
console.log('second: ' + secondSequence.join(', '))