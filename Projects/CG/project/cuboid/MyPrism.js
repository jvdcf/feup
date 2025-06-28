import { CGFobject } from "../../lib/CGF.js";

/**
 * MyPrism
 * @constructor
 * @param scene - Reference to MyScene object
 * @param slices - number of divisions around the Y axis
 * @param stacks - number of divisions along the Y axis
 */
export class MyPrism extends CGFobject {
  constructor(scene, slices, stacks) {
    super(scene);
    this.slices = slices;
    this.stacks = stacks;
    this.initBuffers();
  }

  initBuffers() {
    this.vertices = [];
    this.indices = [];
    this.normals = [];

    for (var i = 0; i < this.slices; i++) {
      // Vertices ===============================================
      const angleStep = (2 * Math.PI) / this.slices;
      const angle = i * angleStep;

      const x0 = Math.cos(angle);
      const y0 = Math.sin(angle);
      const x1 = Math.cos(angle + angleStep);
      const y1 = Math.sin(angle + angleStep);
      
      for (var z = 0; z < this.stacks + 1; z++) {
        this.vertices.push(x0, y0, z);
        this.vertices.push(x1, y1, z);
      }

      // Normals ===============================================
      let normal = [(x1 + x0) / 2, (y1 + y0) / 2, 0];

      const vectorLen = Math.sqrt(
        normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]
      );
      normal[0] /= vectorLen;
      normal[1] /= vectorLen;
      normal[2] /= vectorLen;

      for (var z = 0; z < this.stacks + 1; z++) {
        this.normals.push(...normal);
        this.normals.push(...normal);
      }

      // Indices ===============================================
      for (var z = 0; z < this.stacks; z++) {
        const offset = 2 * (this.stacks + 1) * i + 2 * z;
        this.indices.push(offset + 1, offset + 2, offset + 0);
        this.indices.push(offset + 1, offset + 3, offset + 2);
        this.indices.push(offset + 0, offset + 2, offset + 1);
        this.indices.push(offset + 2, offset + 3, offset + 1);
      }
    }

    this.primitiveType = this.scene.gl.TRIANGLES;
    this.initGLBuffers();
  }
  /**
   * Called when user interacts with GUI to change object's complexity.
   * @param {integer} complexity - changes number of slices
   */
  updateBuffers(complexity) {
    this.slices = 3 + Math.round(9 * complexity); // complexity varies 0-1, so slices varies 3-12
    this.stacks = 1 + Math.round(3 * complexity); // complexity varies 0-1, so stacks varies 1-4

    // reinitialize buffers
    this.initBuffers();
    this.initNormalVizBuffers();
  }
}
