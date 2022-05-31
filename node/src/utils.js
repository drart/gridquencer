/// todo: is there an infusion way of doing this? 
// https://stackoverflow.com/questions/201183/how-to-determine-equality-for-two-javascript-objects
function testTwoObjects(object1, object2){
    return Object.keys(object1).every((key) =>  object1[key] === object2[key]);
}

function clamp (num, min, max){
    return Math.min(Math.max(num, min), max);
}
