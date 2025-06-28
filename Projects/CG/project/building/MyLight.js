import { CGFobject, CGFappearance } from "../../lib/CGF.js";
import { MySphere } from "../panorama/MySphere.js";

const radius = 1;
const ambient = [0.7, 0.3, 0.0, 1];
const diffuse = [0.7, 0.3, 0.0, 1];
const specular = [0.2, 0.2, 0.2, 1];
let lightMaterial = null;

function degToRad(deg) {
  return (deg * Math.PI) / 180;
}

export class MyLight extends CGFobject {
  constructor(scene) {
    super(scene);
    this.sphere = new MySphere(this.scene, 4, 4, false);
    this.radius = radius;
    this.animDuration = 700;

    if (lightMaterial === null) {
      lightMaterial = new CGFappearance(scene);
      /* lightMaterial.setAmbient(...ambient);
      lightMaterial.setDiffuse(...diffuse);
      lightMaterial.setSpecular(...specular); */
    }
  }

  update(input) {
    lightMaterial.setEmission(Math.sin(input));
  }

  turnOff() {
    lightMaterial.setEmission(10);
  }

  display() {
    this.scene.pushMatrix();
    lightMaterial.apply();
    this.scene.scale(radius, radius, radius);
    this.sphere.display();
    this.scene.popMatrix();
  }
}
