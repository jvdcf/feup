import { CGFobject } from "../lib/CGF.js";
import { MyDiamond } from "./MyDiamond.js";
import { MyTriangle } from "./MyTriangle.js";
import { MyTriangleBig } from "./MyTriangleBig.js";
import { MyTriangleSmall } from "./MyTriangleSmall.js";
import { MyParallelogram } from "./MyParallelogram.js";

function degToRad(deg) {
    return deg * Math.PI / 180;
}

/**
 * MyTangram
 * @constructor
 * @param scene - Reference to MyScene object
 */
export class MyTangram extends CGFobject {
    constructor(scene) {
        super(scene);
        this.small = new MyTriangleSmall(scene);
        this.big = new MyTriangleBig(scene);
        this.triangle = new MyTriangle(scene);
        this.parallelogram = new MyParallelogram(scene);
        this.diamond = new MyDiamond(scene);
    }

    changeColor(r, g, b) {
        this.scene.setAmbient(r, g, b, 1);
        this.scene.setDiffuse(r, g, b, 1);
        this.scene.setSpecular(r, g, b, 1);
    }


    greenDiamond() {
        this.scene.pushMatrix();

        const deltaX = 2 - Math.sqrt(0.5);
        const deltaY = 3.4 - Math.sqrt(0.5);
        const translation = [
            1  , 0  , 0, 0,
            0  , 1  , 0, 0,
            0  , 0  , 1, 0,
            deltaX, deltaY, 0, 1
        ];
        this.scene.multMatrix(translation);

        const rotation = [
             Math.cos(degToRad(45)), Math.sin(degToRad(45)), 0, 0,
            -Math.sin(degToRad(45)), Math.cos(degToRad(45)), 0, 0,
            0                     , 0                      , 1, 0,
            0                     , 0                      , 0, 1
        ];
        this.scene.multMatrix(rotation);
        this.changeColor(0, 1, 0);
        this.diamond.display();
        this.scene.popMatrix();
    }

    pinkTriangle() {
        this.scene.pushMatrix();

        this.scene.translate(-0.8, 3.4, 0);
        this.scene.rotate(degToRad(-135), 0, 0, 1);

        this.changeColor(1, 0.62, 0.83);
        this.triangle.display();
        this.scene.popMatrix();
    }

    yellowParallelogram() {
        this.scene.pushMatrix();
        
        this.scene.translate(2, 3.4, 0);
        this.scene.rotate(degToRad(135), 0, 0, 1);

        this.changeColor(1, 1, 0);
        this.parallelogram.display();
        this.scene.popMatrix();
    }

    blueTriangle() {
        this.scene.pushMatrix();

        this.scene.translate(0, 2, 0);
        this.scene.rotate(degToRad(180), 0, 0, 1);

        this.changeColor(0, 0.62, 1);
        this.big.display();
        this.scene.popMatrix();
    }

    redTriangle() {
        this.scene.pushMatrix();
        
        this.scene.translate(-1, 0, 0);
        this.scene.rotate(degToRad(-90), 0, 0, 1);

        this.changeColor(1, 0.05, 0.05)
        this.small.display();
        this.scene.popMatrix();
    }

    orangeTriangle() {
        this.scene.pushMatrix();
        
        this.scene.translate(1, -1, 0);
        this.scene.rotate(degToRad(90), 0, 0, 1);

        this.changeColor(1, 0.62, 0);
        this.big.display();
        this.scene.popMatrix();
    }

    purpleTriangle() {
        this.scene.pushMatrix();
        
        this.scene.translate(0.3, -2.3, 0);
        this.scene.rotate(degToRad(135), 0, 0, 1);

        this.changeColor(0.67, 0.31, 0.77);
        this.small.display();
        this.scene.popMatrix();
    }


    display() {
        this.blueTriangle();
        this.greenDiamond();
        this.yellowParallelogram();
        this.pinkTriangle();
        this.redTriangle();
        this.orangeTriangle();
        this.purpleTriangle();
    }
}
