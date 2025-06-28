import { CGFobject, CGFappearance, CGFtexture, CGFshader } from "../../lib/CGF.js";
import { MyPlane } from "../plane/MyPlane.js";
import { MyLight } from "./MyLight.js";

const width = 10;
const height = 10;
const depth = 1;
/* const ambient = [0.5, 0.5, 0.5, 1];
const diffuse = [1, 1, 1, 1];
const specular = [1, 1, 1, 1];
const shininess = 10; */
const texturePath = "images/heliport.png";
const upTexturePath = "images/heliport_up.png";
const downTexturePath = "images/heliport_down.png";
let heliportTexture = null;
let upTexture = null;
let downTexture = null;

const heliportState = Object.freeze({
  NONE: 1,
  UP: 2,
  DOWN: 3,
});

function degToRad(deg) {
  return (deg * Math.PI) / 180;
}

/**
 * MyHeliport: A rectangle (10 * 1 * 10) used by MyBuilding as a helipot at the
 * top of the building. This object is supposed to be scaled later by
 * MyBuildingModule to fit at the top.
 * @constructor
 * @param scene - Reference to MyScene object
 */
export class MyHeliport extends CGFobject {
  constructor(scene) {
    super(scene);
    this.plane = new MyPlane(scene);
    this.light = new MyLight(scene);
    this.width = width;
    this.height = height;
    this.depth = depth;
    this.state = heliportState.NONE;
    this.animDuration = 700;
    this.animStart = Date.now();
    this.timeFactor = 0;
    this.shader = new CGFshader(this.scene.gl, "shaders/heliport.vert", "shaders/heliport.frag");

    if (heliportTexture === null) {
      heliportTexture = new CGFtexture(scene, texturePath);
    }

    if (upTexture === null) {
      upTexture = new CGFtexture(scene, upTexturePath);
    }

    if (downTexture === null) {
      downTexture = new CGFtexture(scene, downTexturePath);
    }
  }

  update(t) {
    if (this.state === heliportState.NONE) return;
    const animDelta = t - this.animStart;
    const input = (animDelta / this.animDuration) * 2 * Math.PI;
    this.timeFactor = Math.sin(input);
    this.light.update(input);
  }

  stopAnim() {
    this.state = heliportState.NONE;
    this.light.turnOff();
  }

  startUpAnim() {
    this.state = heliportState.UP;
  }

  startDownAnim() {
    this.state = heliportState.DOWN;
  }

  display() {
    this.scene.pushMatrix();
    this.scene.setActiveShader(this.shader);

    heliportTexture.bind(1);
    upTexture.bind(heliportState.UP);
    downTexture.bind(heliportState.DOWN);

    this.shader.setUniformsValues({
      timeFactor: this.timeFactor,
      uSamplerDefault: 1,
      uSamplerAction: this.state,
    });

    this.scene.scale(width, height, depth); // Default proportions
    this.plane.display();
    this.scene.setActiveShader(this.scene.defaultShader);
    this.scene.popMatrix();

    this.scene.pushMatrix();
    this.scene.translate(width / 2, height / 2, 0);
    this.light.display();
    this.scene.popMatrix();
  }
}
