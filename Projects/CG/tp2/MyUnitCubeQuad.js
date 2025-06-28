import {CGFobject} from '../lib/CGF.js';
import {MyQuad} from './MyQuad.js';


function degToRad(deg) {
    return deg * Math.PI / 180;
}
/**
 * MyUnitCubeQuad
 * @constructor
 * @param scene - Reference to MyScene object
 */
export class MyUnitCubeQuad extends CGFobject {
	constructor(scene) {
		super(scene);
		this.quad = new MyQuad(scene);
	}

  frontback() {
    this.scene.pushMatrix();
    this.scene.translate(0, 0, 0.5);

    this.quad.display();
    this.scene.translate(0, 0, -1);
    this.quad.display();
    this.scene.translate(0, 0, 0.5);
    this.scene.popMatrix();
  }
  
  sides() {
    this.scene.pushMatrix();
    this.scene.rotate(degToRad(90),0,1,0);
    this.scene.translate(0, 0, 0.5);
    this.quad.display();
    this.scene.translate(0, 0, -1);
    this.quad.display();
    this.scene.translate(0, 0, 0.5);
    this.scene.rotate(0,degToRad(-90),0,1,0);
    this.scene.popMatrix();
  }
  
  lids() {
    this.scene.pushMatrix();
    this.scene.rotate(degToRad(90),1,0,0);
    this.scene.translate(0, 0, 0.5);
    this.quad.display();
    this.scene.translate(0, 0, -1);
    this.quad.display();
    this.scene.translate(0, 0, 0.5);
    this.scene.rotate(0,degToRad(-90),1,0,0);
    this.scene.popMatrix();
  }
	
	display() {
	  this.frontback();
	  this.sides();
	  this.lids();
	  
	}

}

