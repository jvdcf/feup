import { CGFobject } from "../../lib/CGF.js";

/**
 * MyPyramid (based from TP3)
 * @constructor
 * @param scene - Reference to MyScene object
 * @param {integer} verticeCount - Number of divisions around the base.
 * @param {integer} radius - Radius of the base.
 * @param {integer} height - Height of the pyramid.
 * @param {boolean} rounded - Whether the pyramid resembles a circle or a polygon.
 * If true, the lightning is calculated with "Gourand Shading", else, "Constant
 * Shading" is applied.
 */
export class MyPyramid extends CGFobject {
  constructor(scene, verticeCount, radius, height, rounded) {
    super(scene);
    this.verticeCount = verticeCount; // = slices
    this.radius = radius;
    this.height = height;
    this.rounded = rounded;
    this.initBuffers();
  }

  initBuffers() {
    this.vertices = [];
    this.indices = [];
    this.normals = [];
    this.texCoords = [];

    let ang = 0;
    const alphaAng = (2 * Math.PI) / this.verticeCount;

    for (let i = 0; i < this.verticeCount; i++) {
      // Vertices =============================================================

      // All vertices have to be declared for a given face
      // even if they are shared with others, as the normals
      // in each face will be different

      const sa = Math.sin(ang) * this.radius;
      const saa = Math.sin(ang + alphaAng) * this.radius;
      const ca = Math.cos(ang) * this.radius;
      const caa = Math.cos(ang + alphaAng) * this.radius;

      this.vertices.push(0, this.height, 0);
      this.vertices.push(ca, 0, -sa);
      this.vertices.push(caa, 0, -saa);

      // Normals ==============================================================

      if (this.rounded) {
        this.normals.push(0, 1, 0);
        this.normals.push(ca / this.radius, 0, -sa / this.radius);
        this.normals.push(caa / this.radius, 0, -saa / this.radius);
      } else {
        // triangle normal computed by cross product of two edges
        let normal = [saa - sa, ca * saa - sa * caa, caa - ca];

        // normalization
        let nsize = Math.sqrt(
          normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]
        );
        normal[0] /= nsize;
        normal[1] /= nsize;
        normal[2] /= nsize;

        // push normal once for each vertex of this triangle
        this.normals.push(...normal);
        this.normals.push(...normal);
        this.normals.push(...normal);
      }

      // Indices ==============================================================

      this.indices.push(3 * i, 3 * i + 1, 3 * i + 2);

      // Texture coordinates ==================================================

      this.texCoords.push(0, this.height);
      this.texCoords.push(i, 0);
      this.texCoords.push(i + 1, 0);

      ang += alphaAng;
    }

    this.primitiveType = this.scene.gl.TRIANGLES;
    this.initGLBuffers();
  }

  /**
   * Called when user interacts with GUI to change object's complexity.
   * @param {integer} verticeCount - Number of divisions around the base.
   * @param {integer} radius - Radius of the base.
   * @param {integer} height - Height of the pyramid.
   * @param {boolean} rounded - Whether the pyramid resembles a circle or a polygon.
   * If true, the lightning is calculated with "Gourand Shading", else, "Constant
   * Shading" is applied.
   */
  updateBuffers(verticeCount, radius, height, rounded) {
    this.verticeCount = verticeCount;
    this.radius = radius;
    this.height = height;
    this.rounded = rounded;

    // reinitialize buffers
    this.initBuffers();
    this.initNormalVizBuffers();
  }
}
