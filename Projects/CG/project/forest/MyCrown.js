import { CGFobject, CGFappearance } from "../../lib/CGF.js";
import { MyPyramid } from "../pyramid/MyPyramid.js";

function degToRad(deg) {
  return (deg * Math.PI) / 180;
}

/**
 * MyCrown: A pyramid with green material used by MyTree.
 * @constructor
 * @param scene - Reference to MyScene object
 * @param {integer} radius - Radius of the base.
 * @param {integer} height - Height of the pyramid.
 * @param {array<integer>[3]} color - Color RGB of the pyramid.
 * @param {CGFappearance} texture - A CGFappearance object with **texture only**.
 */
export class MyCrown extends CGFobject {
  constructor(scene, radius, height, color, texture) {
    super(scene);
    this.radius = radius;
    this.height = height;
    this.verticeCount = 6;
    this.pyramid = new MyPyramid(
      scene,
      this.verticeCount,
      radius,
      height,
      false
    );
    this.color = color;
    this.material = texture;
  }

  display() {
    this.scene.pushMatrix();
    const [r, g, b] = this.color;
    this.material.setAmbient(r, g, b, 0.5);
    this.material.setDiffuse(r, g, b, 1);
    this.material.setSpecular(r, g, b, 0);
    this.material.setShininess(10.0);
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
}
