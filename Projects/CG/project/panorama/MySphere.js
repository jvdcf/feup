import { CGFobject } from "../../lib/CGF.js";

/**
 * MySphere: Sky-Sphere
 * @constructor
 * @param scene - Reference to MyScene object
 * @param {integer} slices - Number of "sides" along the Y-axis
 * @param {integer} stacks - Number of equal angular divisions from the "equator line" to the "poles"
 * @param {boolean} isInverted - If true, the sphere will be inside out, i.e., can be seen from the inside
 */
export class MySphere extends CGFobject {
  constructor(scene, slices, stacks, isInverted) {
    super(scene);
    this.slices = slices + 1; // An extra vertice is needed to close the circle
    this.stacks = stacks;
    this.isInverted = isInverted;
    this.initBuffers();
  }

  initBuffers() {
    this.vertices = [];
    this.vertices.push(0, 0, -1);

    this.indices = [];

    this.normals = [];
    if (this.isInverted) this.normals.push(0, 1, 0);
    else this.normals.push(0, 0, -1);

    this.texCoords = [1, 1];

    const yStep = (2.0 * Math.PI) / (this.slices - 1); // Rotation step in Y-axis
    const zStep = Math.PI / 2.0 / this.stacks; // Rotation step in Z-axis

    for (let stackIndex = 0; stackIndex <= 2 * this.stacks; stackIndex++) {
      const zAngle = -(Math.PI / 2.0) + stackIndex * zStep;
      const z = Math.sin(zAngle);

      for (let sliceIndex = 0; sliceIndex < this.slices; sliceIndex++) {
        const yAngle = sliceIndex * yStep;

        // Vertices ===============================================
        const x = Math.sin(yAngle) * Math.cos(zAngle);
        const y = Math.cos(yAngle) * Math.cos(zAngle);
        this.vertices.push(x, y, z);

        // Indices ================================================
        const verticeIndex = stackIndex * this.slices + sliceIndex + 1;

        /** Lower triangle: (viewed from outside of the sphere)
         * ```
         *     i - 1  *
         *            | \
         * i - s - 1  * - *  i - s
         * ```
         */
        let lowerTriangle = [
          clamp(0, Infinity, verticeIndex - this.slices),
          clamp(0, Infinity, verticeIndex - this.slices - 1),
          verticeIndex - 1,
        ];

        /** Higher triangle: (viewed from outside of the sphere)
         * ```
         * i - 1  * - *  i
         *          \ |
         *            *  i - s
         * ```
         */
        let higherTriangle = [
          verticeIndex,
          clamp(0, Infinity, verticeIndex - this.slices),
          verticeIndex - 1,
        ];

        if (this.isInverted) {
          lowerTriangle = lowerTriangle.reverse();
          higherTriangle = higherTriangle.reverse();
        }

        this.indices.push(...lowerTriangle);
        this.indices.push(...higherTriangle);

        // Normals ================================================
        let normal = [x, y, z];
        const vectorLen = Math.sqrt(
          Math.pow(normal[0], 2) +
            Math.pow(normal[1], 2) +
            Math.pow(normal[2], 2)
        );
        normal = normal.map((n) => n / vectorLen);
        if (this.isInverted) normal = normal.map((n) => -n);
        this.normals.push(normal[0], normal[1], normal[2]);

        // Texture coordinates ====================================
        const u = 1 - sliceIndex / (this.slices - 1);
        const v = 1 - stackIndex / (2 * this.stacks);
        this.texCoords.push(u, v);
      }
    }

    this.primitiveType = this.scene.gl.TRIANGLES; // The defined indices (and corresponding vertices) will be read in groups of three to draw triangles.
    this.initGLBuffers();
  }

  /**
   * Called when user interacts with GUI to change object's complexity.
   * @param {integer} slices - changes number of slices
   * @param {integer} stacks - changes number of stacks
   */
  updateBuffers(slices, stacks) {
    this.slices = slices + 1;
    this.stacks = stacks;

    // reinitialize buffers
    this.initBuffers();
    this.initNormalVizBuffers();
  }
}

function clamp(min, max, v) {
  if (min > max) {
    throw new Error("clamp: max cannot be smaller than min.");
  }
  try {
    return Math.max(min, Math.min(max, v));
  } catch (e) {
    return min;
  }
}
