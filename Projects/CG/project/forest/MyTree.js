import { CGFappearance, CGFobject } from "../../lib/CGF.js";
import { MyCrown } from "./MyCrown.js";
import { MyTrunk } from "./MyTrunk.js";

export class AXIS {
  // Class used as an enum because JavaScript... :/
  static X = "X";
  static Z = "Z";
}

function degToRad(deg) {
  return (deg * Math.PI) / 180;
}

/**
 * MyTree: A single tree with a trunk and a crown
 * @constructor
 * @param scene - Reference to MyScene object
 * @param {integer} tiltAngle - Rotation in degrees around the tiltAxis.
 * 0 corresponds to the vertical position.
 * @param {AXIS | string} tiltAxis - Axis of rotation: 'X' or 'Z'.
 * @param {integer} radius - Radius of the trunk base.
 * @param {integer} height - Height of the tree.
 * @param {array<integer>[3]} color - Color of the crown in RGB format.
 * @param {CGFappearance} crownTexture - A CGFappearance object for the crown
 * with **texture only**.
 * @param {CGFappearance} trunkTexture - A CGFappearance object for the trunk
 * with **texture only**.
 */
export class MyTree extends CGFobject {
  constructor(scene, tiltAngle, tiltAxis, radius, height, color, crownTexture, trunkTexture) {
    super(scene);
    // The height of the crown can be 80% of the height of the tree
    this.crownHeightRatio = 0.8;
    // Height units of a pyramid in the crown
    this.pyramidHeight = 20;
    // Ratio of visibility of the pyramid when placed with the other crowns
    this.pyramidVisibleRatio = 0.5;
    // Radius of the pyramid based on the radius of the trunk in the y coordinate of the pyramid base
    this.pyramidToTrunkRadiusRatio = 2.5;

    this.tiltAngle = tiltAngle;
    this.tiltAxis = tiltAxis;
    this.radius = radius;
    this.height = height;
    this.color = color;
    this.crownTexture = crownTexture;
    this.trunkTexture = trunkTexture;

    this.trunk = new MyTrunk(scene, radius, height, this.trunkTexture);
    this.crown = this.generateCrownPyramids();
  }

  generateCrownPyramids() {
    let crowns = [];
    const zStart = this.height * (1 - this.crownHeightRatio);
    const zStep = this.pyramidHeight * this.pyramidVisibleRatio;
    const zEnd = this.height - this.pyramidHeight;
    for (let z = zEnd; z >= zStart; z -= zStep) {
      const radiusPerHeight = this.radius / this.height;
      const pyramidRadius =
        (this.height - z) * radiusPerHeight * this.pyramidToTrunkRadiusRatio;
      const pyramid = new MyCrown(
        this.scene,
        pyramidRadius,
        this.pyramidHeight,
        this.color,
        this.crownTexture
      );
      crowns.push(pyramid);
    }
    return crowns;
  }

  display() {
    this.scene.pushMatrix();

    // TODO: Fix Tilt in Z axis
    const xTilt = (this.tiltAxis === AXIS.X) ? 1 : 0;
    const zTilt = (this.tiltAxis === AXIS.Z) ? 1 : 0;
    this.scene.rotate(degToRad(this.tiltAngle), xTilt, 0, zTilt);

    this.trunk.display();
    const zStep = this.pyramidHeight * this.pyramidVisibleRatio;
    let z = this.height - this.pyramidHeight;

    for (const pyramid of this.crown) {
      this.scene.pushMatrix();
      this.scene.translate(0, 0, z);
      pyramid.display();
      z -= zStep;
      this.scene.popMatrix();
    }

    this.scene.popMatrix();
  }

  enableNormalViz() {
    this.trunk.enableNormalViz();
    this.crown.forEach(c => c.enableNormalViz());
  }
  
  disableNormalViz() {
    this.trunk.disableNormalViz();
    this.crown.forEach(c => c.disableNormalViz());
  }
}
