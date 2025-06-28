import { CGFobject, CGFappearance, CGFshader } from "../../lib/CGF.js";
import { MyTriangle } from "../2d/MyTriangle.js";

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

// https://easings.net/#easeInOutSine
function easeInOutSine(x) {
  return -(Math.cos(Math.PI * x) - 1) / 2;
}


export class MyFire extends CGFobject {
  constructor(scene, treeRadius, fireMaterial) {
    super(scene);
    this.triangle = new MyTriangle(scene, new Range(5, 20).getRand(), new Range(10, 40).getRand());
    this.scale = 1;
    this.startAnim = undefined;
    this.animDuration = undefined;
    this.rotateAngle = new Range(0, 2 * Math.PI).getRand();
    this.treeRadius = treeRadius;
    this.fireMaterial = fireMaterial;
    this.z = new Range(10, 50).getRand();

    
  }

  update(t) {

    if (this.startAnim === undefined || this.animDuration === undefined) return;
    const elapsedTime = t - this.startAnim;
    const endAnim = this.startAnim + this.animDuration;

    if (t < endAnim && t >= this.startAnim) {
      this.scale = 1 - easeInOutSine(elapsedTime / this.animDuration);
    }
  }

  snub() {
    this.startAnim = Date.now();
    this.animDuration = new Range(1000, 5000).getRand();
  }

  display() {
    this.scene.pushMatrix();
    this.scene.scale(this.scale, this.scale, this.scale);
    this.scene.translate(0, 0, this.z);
    this.scene.rotate(Math.PI / 2, 1, 0, 0);
    this.scene.rotate(this.rotateAngle, 0, 1, 0);
    this.scene.translate(this.treeRadius, 0, 0);
    // this.fireMaterial.apply();
    this.triangle.display();
    this.scene.popMatrix();
  }
}
