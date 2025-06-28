import { CGFobject, CGFappearance, CGFshader } from "../../lib/CGF.js";
import { MyTree, AXIS } from "./MyTree.js";
import { MyFire } from "./MyFire.js";

class TreeCell {
  constructor(tree, x, y, fire = undefined) {
    this.tree = tree;
    this.fire = fire;
    this.x = parseFloat(x);
    this.y = parseFloat(y);
  }
}

class Range {
  constructor(min, max) {
    this.min = min;
    this.max = max;
  }

  getRand() {
    return Math.random() * (this.max - this.min) + this.min;
  }
}

function degToRad(deg) {
  return (deg * Math.PI) / 180;
}

var fireShader = null;

/**
 * MyForest: A set of randomly selected trees in a grid.
 * @constructor
 * @param scene - Reference to MyScene object
 * @param {integer} width - Number of trees in the x direction.
 * @param {integer} height - Number of trees in the y direction.
 */
export class MyForest extends CGFobject {
  constructor(scene, width, height) {
    super(scene);
    this.width = width;
    this.height = height;
    this.cellSize = 30;
    this.tiltAngleRange = new Range(-10, 10);
    this.tiltAxisRange = new Range(-1, 1);  // < 0 -> AXIS.X; > 0 -> AXIS.Z
    this.radiusRange = new Range(5, 10);
    this.heightRange = new Range(50, 100);
    this.rRange = new Range(0.02, 0.1921568627450980);
    this.gRange = new Range(0.15, 0.3294117647058824);
    this.bRange = new Range(0.01, 0.1450980392156863);
    this.xDeltaRange = new Range(-5, 5);
    this.yDeltaRange = new Range(-5, 5);
    this.fireRange = new Range(0, 1);
    this.fireProb = 0.50;
    this.snubRadius = 90;

    this.crownTexture = new CGFappearance(this.scene);
    this.crownTexture.loadTexture("./images/tree.jpg");
    this.crownTexture.setTextureWrap("REPEAT", "REPEAT");

    this.trunkTexture = new CGFappearance(this.scene);
    this.trunkTexture.loadTexture("./images/tree.jpg");
    this.trunkTexture.setTextureWrap("REPEAT", "REPEAT");

    this.fireTexture = new CGFappearance(this.scene);
    this.color = [new Range(150, 255).getRand(), new Range(50, 150).getRand(), 0];
    this.updateFireColor(this.color);

    this.generateElements();
    
    if (fireShader === null) {
      fireShader = new CGFshader(scene.gl, "shaders/fire.vert", "shaders/fire.frag");
    }

    this.counter = 0;
  }

  generateElements() {
    this.treeCells = [];
    for (let i = 0; i < this.width * this.cellSize; i += this.cellSize) {
      for (let j = 0; j < this.height * this.cellSize; j += this.cellSize) {
        const tree = new MyTree(
          this.scene,
          this.tiltAngleRange.getRand(),
          (this.tiltAxisRange.getRand() < 0) ? AXIS.X : AXIS.Z,
          this.radiusRange.getRand(),
          this.heightRange.getRand(),
          [
            this.rRange.getRand(),
            this.gRange.getRand(),
            this.bRange.getRand(),
          ],
          this.crownTexture,
          this.trunkTexture
        );

        let fire = undefined;
        if (this.fireRange.getRand() <= this.fireProb) {
          fire = new MyFire(this.scene, tree.radius, this.fireTexture);
        }

        const tCell = new TreeCell(
          tree, 
          i + this.xDeltaRange.getRand(),
          j + this.yDeltaRange.getRand(),
          fire
        );
        this.treeCells.push(tCell);
      }
    }
  }

  display() {
    this.scene.pushMatrix();
    
    for (const t of this.treeCells) {
      this.scene.pushMatrix();
      this.scene.translate(t.x, t.y, 0);
      t.tree.display();
      this.scene.pushMatrix();
      const xTilt = (this.tiltAxis === AXIS.X) ? 1 : 0;
      const zTilt = (this.tiltAxis === AXIS.Z) ? 1 : 0;
      this.scene.rotate(degToRad(t.tree.tiltAngle), xTilt, 0, zTilt);
      this.scene.popMatrix();
      this.scene.popMatrix();
    }

    this.scene.setActiveShader(fireShader);
    for (const t of this.treeCells) {
      this.scene.pushMatrix();
      this.scene.translate(t.x, t.y, 0);
      this.scene.pushMatrix();
      const xTilt = (this.tiltAxis === AXIS.X) ? 1 : 0;
      const zTilt = (this.tiltAxis === AXIS.Z) ? 1 : 0;
      this.scene.rotate(degToRad(t.tree.tiltAngle), xTilt, 0, zTilt);
      t.fire?.display();
      this.scene.popMatrix();
      this.scene.popMatrix();
    }
    this.scene.setActiveShader(this.scene.defaultShader);
    

    this.scene.popMatrix();
  }

  snubFire(x, y) {
    for (const tCell of this.treeCells) {
      if (tCell.fire === undefined) continue;
      const deltaX = tCell.x + (this.cellSize / 2) - x;
      const deltaY = tCell.y + (this.cellSize / 2) - y;
      const distance = Math.sqrt(deltaX * deltaX + deltaY * deltaY);
      if (distance < this.snubRadius) {
        tCell.fire.snub();
      }
    }
  }

  update(t) {
    fireShader.setUniformsValues({
      timeFactor: t / 300 % 1000,
      random: this.counter,
    });

    this.counter += 0.1;
    this.counter %= 1000;
    
    for (const tCell of this.treeCells) {
      if (tCell.fire === undefined) continue;
      if (tCell.fire.scale < 0.05) {
        tCell.fire = undefined;
        continue;
      }
      tCell.fire?.update(t);
    }
  }

  updateBuffers(width, height, fireProb) {
    this.width = width;
    this.height = height;
    this.fireProb = fireProb;
    this.generateElements();
  }

  updateFireColor(color) {
    this.color = color;
    const [r, g, b] = this.color.map((c) => c / 255);
    this.fireTexture.setAmbient(r, g, b, 1);
    this.fireTexture.setDiffuse(r, g, b, 1);
    this.fireTexture.setSpecular(r / 10, g / 10, b / 10, 1);
    this.fireTexture.setEmission(r, g, b, 1);
    this.fireTexture.loadTexture("images/tree.jpg");
    this.fireTexture.setTextureWrap("REPEAT", "REPEAT");
    this.fireTexture.setShininess(10);
  }
}
