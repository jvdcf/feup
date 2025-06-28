import { CGFobject, CGFappearance } from "../../lib/CGF.js";
import { MyPlane } from "../plane/MyPlane.js";

const width = 15;
const height = 5;
const depth = 1;
const ambient = [0.5, 0.5, 0.5, 1];
const diffuse = [1, 1, 1, 1];
const specular = [1, 1, 1, 1];
const shininess = 10;
const texturePath = "images/door_sign.jpg";
let doorSignMaterial = null;

function degToRad(deg) {
  return (deg * Math.PI) / 180;
}

/**
 * MyDoorSign: A rectangle (15 * 5 * 1) used by MyBuilding as a door sign
 * @constructor
 * @param scene - Reference to MyScene object
 */
export class MyDoorSign extends CGFobject {
  constructor(scene) {
    super(scene);
    this.plane = new MyPlane(scene);
    this.width = width;
    this.height = height;
    this.depth = depth;

    if (doorSignMaterial === null) {
      doorSignMaterial = new CGFappearance(scene);
      doorSignMaterial.setAmbient(...ambient);
      doorSignMaterial.setDiffuse(...diffuse);
      doorSignMaterial.setSpecular(...specular);
      doorSignMaterial.setShininess(shininess);
      doorSignMaterial.loadTexture(texturePath);
      doorSignMaterial.setTextureWrap("REPEAT", "REPEAT");
    }
  }

  display() {
    this.scene.pushMatrix();
    doorSignMaterial.apply();
    this.scene.rotate(degToRad(90), 1, 0, 0); // Rotate to face the camera
    this.scene.scale(width, height, depth); // Default proportions
    this.plane.display();
    this.scene.popMatrix();
  }
}
