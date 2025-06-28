import { CGFobject, CGFappearance } from "../../lib/CGF.js";
import { MyCircle } from "../2d/MyCircle.js";

let lakeMaterial = null;

class Range {
  constructor(min, max) {
    this.min = min;
    this.max = max;
  }

  getRand() {
    return Math.random() * (this.max - this.min) + this.min;
  }
}

export class MyLake extends CGFobject {
  constructor(scene) {
    super(scene);
    this.circle = new MyCircle(scene, 1, 30);
    this.width = 210;
    this.height = 80;
    this.z = new Range(10, 50).getRand();
    if (lakeMaterial === null) {
      lakeMaterial = new CGFappearance(scene);
      const [r, g, b] = [25, 40, 255].map(x => x / 255);
      lakeMaterial.setAmbient(r, g, b, 1);
      lakeMaterial.setDiffuse(r, g, b, 1);
      lakeMaterial.setSpecular(r / 10, g / 10, b / 10, 1);
      lakeMaterial.setEmission(r, g, b, 1);
      lakeMaterial.loadTexture("images/tree.jpg");
      lakeMaterial.setTextureWrap("REPEAT", "REPEAT");
      lakeMaterial.setShininess(10);
    }
  }

  display() {
    this.scene.pushMatrix();
    this.scene.scale(this.width, this.height, 1);
    lakeMaterial.apply();
    this.circle.display();
    this.scene.popMatrix();
  }

  updateBuffers(width, height) {
    this.width = width;
    this.height = height;
  }
}
