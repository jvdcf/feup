import { CGFobject, CGFappearance } from "../../lib/CGF.js";
import { MyUnitCube } from "./MyUnitCube.js";

/**
 * MyCuboid: A rectangular parallelepiped with a single color and a diffuse texture.
 * @constructor
 * @param scene - Reference to MyScene object
 * @param {integer} x - Width of the cuboid
 * @param {integer} y - Height of the cuboid
 * @param {integer} z - Depth of the cuboid
 * @param {array<integer>[3]} color - RGB color of the cuboid
 */
export class MyCuboid extends CGFobject {
  constructor(scene, x, y, z, color) {
    super(scene);
    this.cube = new MyUnitCube(scene);
    this.x = x;
    this.y = y;
    this.z = z;
    this.material = new CGFappearance(scene);
    const [r, g, b] = color;
    this.material.setAmbient(r, g, b, 0.3);
    this.material.setDiffuse(r, g, b, 0.6);
    this.material.setSpecular(r, g, b, 0.1);
    this.material.setShininess(10.0);
  }

  display() {
    this.scene.pushMatrix();
    this.material.apply();
    this.scene.scale(this.x, this.y, this.z);
    this.cube.display();
    this.scene.popMatrix();
  }

  enableNormalViz() {
    this.cube.enableNormalViz();
  }

  disableNormalViz() {
    this.cube.disableNormalViz();
  }

  updateBuffers(x, y, z, color) {
    this.x = x;
    this.y = y;
    this.z = z;
    this.material = new CGFappearance(this.scene);
    const [r, g, b] = color;
    this.material.setAmbient(r, g, b, 0.3);
    this.material.setDiffuse(r, g, b, 0.6);
    this.material.setSpecular(r, g, b, 0.1);
    this.material.setShininess(10.0);
  }
}
