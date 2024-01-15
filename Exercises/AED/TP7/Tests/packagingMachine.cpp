#include <algorithm>
#include "packagingMachine.h"

PackagingMachine::PackagingMachine(int boxCap): boxCapacity(boxCap)
{}

unsigned PackagingMachine::numberOfBoxes() {
	return boxes.size();
}

unsigned PackagingMachine::addBox(Box& b1) {
	boxes.push(b1);
	return boxes.size();
}

HeapObj PackagingMachine::getObjects() const {
	return this->objects;
}

HeapBox PackagingMachine::getBoxes() const {
	return this->boxes;
}


//=============================================================================
// Exercise 1: Packaging
//=============================================================================
// Subexercise 1.1: Load Objects
//=============================================================================
unsigned PackagingMachine::loadObjects(vector<Object> &objs) {
    unsigned count = 0;
	for (Object obj: objs) {
        if (obj.getWeight() <= boxCapacity) {
            objects.push(obj);
            count++;
        }
    }

    // Remove every object loaded
    objs.erase(remove_if(objs.begin(), objs.end(), [this](Object o) {
        return o.getWeight() <= this->boxCapacity;
    }), objs.end());

    return count;
}


//=============================================================================
// Subexercise 1.2: Search Box
//=============================================================================
Box PackagingMachine::searchBox(Object& obj) {
    int load = obj.getWeight();
    priority_queue<Box> temp;

    while (!boxes.empty()) {
        if (boxes.top().getFree() >= load) {
            Box next_box = boxes.top();
            boxes.pop();
            while (!temp.empty()) {
                boxes.push(temp.top());
                temp.pop();
            }
            return next_box;

        } else {
            temp.push(boxes.top());
            boxes.pop();
        }
    }

    // If no box found:
    while (!temp.empty()) {
      boxes.push(temp.top());
      temp.pop();
    }
    Box next_box = Box();
    return next_box;
}


//=============================================================================
// Subexercise 1.3: Pack Objects
//=============================================================================
unsigned PackagingMachine::packObjects() {
    unsigned size = 0;
    while (!objects.empty()) {
        Object o = objects.top();
        objects.pop();
        Box b = searchBox(o);
        b.addObject(o);
        size = addBox(b);
    }
    return size;
}


//=============================================================================
// Subexercise 1.4: Box with More Objects
//=============================================================================
stack<Object> PackagingMachine::boxWithMoreObjects() const {
    stack<Object> mostObjects;
    int countObjects = 0;
    priority_queue<Box> temp = boxes;

    while (!temp.empty()) {
        if (temp.top().getSize() > countObjects) {
            mostObjects = temp.top().getObjects();
            countObjects = temp.top().getSize();
        }
        
        temp.pop();
    }
    return mostObjects;
}
