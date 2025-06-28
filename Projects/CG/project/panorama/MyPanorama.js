import { CGFappearance, CGFobject } from "../../lib/CGF.js";
import { MySphere } from "./MySphere.js";

/**
 * MyPanorama: Sky-Sphere with paronama texture
 * @constructor
 * @param scene - Reference to MyScene object
 * @param {string} texturePath - Path to the texture image to be applied to the sphere
 */
export class MyPanorama extends CGFobject {
  constructor(scene, texturePath) {
    super(scene);
    this.slices = 8;
    this.stacks = 16;
    this.radius = 400;
    this.sphere = new MySphere(scene, this.stacks, this.slices, true);
    this.texture = new CGFappearance(scene);
    this.texture.setEmission(1, 1, 1, 1);
    this.texture.loadTexture(texturePath);
    this.texture.setTextureWrap("REPEAT", "REPEAT");
  }

  display() {
    this.scene.pushMatrix();
    this.texture.apply();
    this.scene.scale(this.radius, this.radius, this.radius);
    this.sphere.display();
    this.scene.popMatrix();
  }

  enableNormalViz() {
    this.sphere.enableNormalViz();
  }

  disableNormalViz() {
    this.sphere.disableNormalViz();
  }

  updateBuffers(slices, stacks) {
    this.sphere.updateBuffers(slices, stacks);
  }
}
