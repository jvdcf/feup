import { CGFobject, CGFappearance } from "../../lib/CGF.js";
import { MyPyramid } from "../pyramid/MyPyramid.js";

function degToRad(deg) {
  return (deg * Math.PI) / 180;
}

/**
 * MyTrunk: A cone made of brown material used by MyTree.
 * @constructor
 * @param scene - Reference to MyScene object
 * @param {integer} radius - Radius of the trunk base.
 * @param {integer} height - Height of the trunk.
 * @param {CGFappearance} texture - A CGFappearance object with **texture only**.
 */
export class MyTrunk extends CGFobject {
  constructor(scene, radius, height, texture) {
    super(scene);
    this.verticeCount = 2 * radius;
    let [r, g, b] = [97, 76, 66].map(i => i / 255);
    this.pyramid = new MyPyramid(
      scene,
      this.verticeCount,
      radius,
      height,
      true
    );
    this.material = texture;
    this.material.setAmbient(r, g, b, 0.5);
    this.material.setDiffuse(r, g, b, 1);
    this.material.setSpecular(r, g, b, 0);
    this.material.setShininess(10.0);
  }

  display() {
    this.scene.pushMatrix();
    this.material.apply();
    this.scene.rotate(degToRad(90), 1, 0, 0); // Rotate to face the camera
    this.pyramid.display();
    this.scene.popMatrix();
  }

  enableNormalViz() {
    this.pyramid.enableNormalViz();
  }

  disableNormalViz() {
    this.pyramid.disableNormalViz();
  }

  updateBuffers(radius, height) {
    this.radius = radius;
    this.height = height;
    this.pyramid.updateBuffers(
      this.verticeCount,
      this.radius,
      this.height,
      true
    );
  }
}
