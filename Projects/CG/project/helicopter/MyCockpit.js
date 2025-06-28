import { CGFobject, CGFappearance } from "../../lib/CGF.js";
import { MySphere } from "../panorama/MySphere.js";

// Change here
const LENGTH = 0.7;
const VERTICAL_RESOLUTION = 12;
const HORIZONTAL_RESOLUTION = 20;


const RADIUS = LENGTH / 2.;

export class MyCockPit extends CGFobject {
  constructor(scene) {
    super(scene);
    this.len = LENGTH;
    this.radius = RADIUS;
    this.sphere = new MySphere(scene, VERTICAL_RESOLUTION, HORIZONTAL_RESOLUTION, false);
    this.material = new CGFappearance(this.scene);
    let [r,g,b] = [20, 20, 220].map(x => x / 255);
    this.material.setAmbient(r,g,b, 0.5);
    this.material.setDiffuse(r,g,b, 0.7);
    this.material.setSpecular(r,g,b, 0.8);
    this.material.setShininess(20.);
  }

  set(len, xRes, yRes) {
    this.len = len;
    this.radius = len / 2.;
    this.sphere.updateBuffers(yRes, xRes);
  }

  display() {
    this.scene.pushMatrix();
      this.material.apply();
      this.scene.scale(this.len, this.len, this.len);
      this.sphere.display();
    this.scene.popMatrix();
  }
  
  
}
