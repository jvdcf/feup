import { CGFobject, CGFappearance } from "../../lib/CGF.js";
import { MyPlane } from "../plane/MyPlane.js";

const width = 800;
const height = 800;
const depth = 1;
const ambient = [0, 1, 0, 0];
const diffuse = [1, 1, 1, 0];
const specular = [0, 1, 0, 0];
const shininess = 10;
const texturePath = "images/grass.jpg";
let grassMaterial = null;

/**
 * MyWindow: A rectangle (10 * 10 * 1) used by MyBuilding as a window
 * @constructor
 * @param scene - Reference to MyScene object
 */
export class MyGrass extends CGFobject {
  constructor(scene) {
    super(scene);
    this.plane = new MyPlane(scene, 8, 0, 8, 0, 8);
    this.width = width;
    this.height = height;
    this.depth = depth;

    if (grassMaterial === null) {
      grassMaterial = new CGFappearance(scene);
      grassMaterial.setAmbient(...ambient);
      grassMaterial.setDiffuse(...diffuse);
      grassMaterial.setSpecular(...specular);
      grassMaterial.setShininess(shininess);
      grassMaterial.loadTexture(texturePath);
      grassMaterial.setTextureWrap("REPEAT", "REPEAT");
    }
  }

  display() {
    this.scene.pushMatrix();
    grassMaterial.apply();
    this.scene.scale(width, height, depth); // Default proportions
    this.plane.display();
    this.scene.popMatrix();
  }
}
