import { CGFobject, CGFappearance } from "../../lib/CGF.js";
import { MyPlane } from "../plane/MyPlane.js";

const width = 10;
const height = 10;
const depth = 1;
const ambient = [0.5, 0.5, 0.5, 1];
const diffuse = [1, 1, 1, 1];
const specular = [1, 1, 1, 1];
const shininess = 10;
const texturePath = "images/window.jpg";
let windowMaterial = null;

function degToRad(deg) {
  return (deg * Math.PI) / 180;
}

/**
 * MyWindow: A rectangle (10 * 10 * 1) used by MyBuilding as a window
 * @constructor
 * @param scene - Reference to MyScene object
 */
export class MyWindow extends CGFobject {
  constructor(scene) {
    super(scene);
    this.plane = new MyPlane(scene);
    this.width = width;
    this.height = height;
    this.depth = depth;

    if (windowMaterial === null) {
      windowMaterial = new CGFappearance(scene);
      windowMaterial.setAmbient(...ambient);
      windowMaterial.setDiffuse(...diffuse);
      windowMaterial.setSpecular(...specular);
      windowMaterial.setShininess(shininess);
      windowMaterial.loadTexture(texturePath);
      windowMaterial.setTextureWrap("REPEAT", "REPEAT");
    }
  }

  display() {
    this.scene.pushMatrix();
    windowMaterial.apply();
    this.scene.rotate(degToRad(90), 1, 0, 0); // Rotate to face the camera
    this.scene.scale(width, height, depth); // Default proportions
    this.plane.display();
    this.scene.popMatrix();
  }
}
