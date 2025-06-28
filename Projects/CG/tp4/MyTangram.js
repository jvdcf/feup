import { CGFappearance, CGFobject } from "../lib/CGF.js";
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
        this.redMaterial = new CGFappearance(scene);
        this.purpleMaterial = new CGFappearance(scene);
        this.big = new MyTriangleBig(scene);
        this.blueMaterial = new CGFappearance(scene);
        this.orangeMaterial = new CGFappearance(scene);
        this.triangle = new MyTriangle(scene);
        this.pinkMaterial = new CGFappearance(scene);
        this.parallelogram = new MyParallelogram(scene);
        this.yellowMaterial = new CGFappearance(scene);
        this.diamond = new MyDiamond(scene);
        this.greenMaterial = new CGFappearance(scene);
    }

    changeColor(material, r, g, b) {
        // TP4: Replaced by textures
    }


    greenDiamond() {
        this.diamond.updateTexCoords([
			0, 0.5,
			0.25, 0.75,
			0.25, 0.25,
			0.5, 0.5,
			0, 0.5,
			0.25, 0.75,
			0.25, 0.25,
			0.5, 0.5,
		]);

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
        this.diamond.display();
        this.scene.popMatrix();
    }

    pinkTriangle() {
        this.triangle.updateTexCoords([
            0, 1,
            0.5, 1,
            0, 0.5,
            0, 1,
            0.5, 1,
            0, 0.5,
        ]);
        this.scene.pushMatrix();

        this.scene.translate(-0.8, 3.4, 0);
        this.scene.rotate(degToRad(-135), 0, 0, 1);

        this.triangle.display();
        this.scene.popMatrix();
    }

    yellowParallelogram() {
        this.parallelogram.updateTexCoords([
            1, 1,
            0.5, 1,
            0.25, 0.75,
            0.75, 0.75,
            1, 1,
            0.5, 1,
            0.25, 0.75,
            0.75, 0.75,
        ]);
        this.scene.pushMatrix();
        
        this.scene.translate(2, 3.4, 0);
        this.scene.rotate(degToRad(135), 0, 0, 1);

        this.parallelogram.display();
        this.scene.popMatrix();
    }

    blueTriangle() {
        this.big.updateTexCoords([
            0, 0,
            1, 0,
            0.5, 0.5,
            0, 0,
            1, 0,
            0.5, 0.5,
        ]);

        this.scene.pushMatrix();
        
        this.scene.translate(0, 2, 0);
        this.scene.rotate(degToRad(180), 0, 0, 1);

        this.big.display();
        this.scene.popMatrix();
    }

    redTriangle() {
        this.small.updateTexCoords([
            0.25, 0.75,
            0.75, 0.75,
            0.5, 0.5,
            0.25, 0.75,
            0.75, 0.75,
            0.5, 0.5,
        ]);
        this.scene.pushMatrix();
        
        this.scene.translate(-1, 0, 0);
        this.scene.rotate(degToRad(-90), 0, 0, 1);

        this.small.display();
        this.scene.popMatrix();
    }

    orangeTriangle() {
        this.big.updateTexCoords([
            1, 1,
            1, 0,
            0.5, 0.5,
            1, 1,
            1, 0,
            0.5, 0.5,
        ]);

        this.scene.pushMatrix();
        
        this.scene.translate(1, -1, 0);
        this.scene.rotate(degToRad(90), 0, 0, 1);

        this.big.display();
        this.scene.popMatrix();
    }

    purpleTriangle() {
        this.small.updateTexCoords([
            0, 0,
            0, 0.5,
            0.25, 0.25,
            0, 0,
            0, 0.5,
            0.25, 0.25,
        ]);

        this.scene.pushMatrix();
        
        this.scene.translate(0.3, -2.3, 0);
        this.scene.rotate(degToRad(135), 0, 0, 1);

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

    enableNormalViz() {
        this.small.enableNormalViz();
        this.big.enableNormalViz();
        this.triangle.enableNormalViz();
        this.parallelogram.enableNormalViz();
        this.diamond.enableNormalViz();
    }

    disableNormalViz() {
        this.small.disableNormalViz();
        this.big.disableNormalViz();
        this.triangle.disableNormalViz();
        this.parallelogram.disableNormalViz();
        this.diamond.disableNormalViz();
    }
}
