import { CGFobject, CGFappearance } from "../../lib/CGF.js";
import { MyPlane } from "../plane/MyPlane.js";

const width = 8;
const height = 10;
const depth = 1;
const ambient = [0.7, 0.7, 0.7, 1];
const diffuse = [1, 1, 1, 1];
const specular = [0.2, 0.2, 0.2, 1];
const shininess = 100;
const texturePath = "images/door.jpg";
let doorMaterial = null;

function degToRad(deg) {
  return (deg * Math.PI) / 180;
}

/**
 * MyDoor: A rectangle (8 * 10 * 1) used by MyBuilding as a door
 * @constructor
 * @param scene - Reference to MyScene object
 */
export class MyDoor extends CGFobject {
  constructor(scene) {
    super(scene);
    this.plane = new MyPlane(scene);
    this.width = width;
    this.height = height;
    this.depth = depth;

    if (doorMaterial === null) {
      doorMaterial = new CGFappearance(scene);
      doorMaterial.setAmbient(...ambient);
      doorMaterial.setDiffuse(...diffuse);
      doorMaterial.setSpecular(...specular);
      doorMaterial.setShininess(shininess);
      doorMaterial.loadTexture(texturePath);
      doorMaterial.setTextureWrap("REPEAT", "REPEAT");
    }
  }

  display() {
    this.scene.pushMatrix();
    doorMaterial.apply();
    this.scene.rotate(degToRad(90), 1, 0, 0); // Rotate to face the camera
    this.scene.scale(width, height, depth); // Default proportions
    this.plane.display();
    this.scene.popMatrix();
  }
}
