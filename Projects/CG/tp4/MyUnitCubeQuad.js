import {CGFobject, CGFappearance} from '../lib/CGF.js';
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
	constructor(scene, top, front, right, back, left, bottom) {
		super(scene);
		this.quad = new MyQuad(scene);
    this.top = this.buildTexture(scene, top);
    this.front = this.buildTexture(scene, front);
    this.right = this.buildTexture(scene, right);
    this.back = this.buildTexture(scene, back);
    this.left = this.buildTexture(scene, left);
    this.bottom = this.buildTexture(scene, bottom);
	}

  buildTexture(scene, texture) {
    const mat = new CGFappearance(scene);
    mat.setAmbient(0.9, 0.9, 0.9, 1);
    mat.loadTexture(texture);
    return mat;
  }

  applyTexture(scene, texture) {
    texture.apply();
    scene.gl.texParameteri(
      scene.gl.TEXTURE_2D, 
      scene.gl.TEXTURE_MAG_FILTER, 
      scene.gl.NEAREST
    );
  }

  frontback() {
    this.scene.pushMatrix();
    this.scene.translate(0, 0, 0.5);
    this.applyTexture(this.scene, this.front);    
    this.quad.display();
    this.scene.translate(0, 0, -1);
    this.applyTexture(this.scene, this.back);
    this.quad.display();
    this.scene.translate(0, 0, 0.5);
    this.scene.popMatrix();
  }
  
  sides() {
    this.scene.pushMatrix();
    this.scene.rotate(degToRad(90),0,1,0);
    this.scene.translate(0, 0, 0.5);
    this.applyTexture(this.scene, this.right);
    this.quad.display();
    this.scene.translate(0, 0, -1);
    this.applyTexture(this.scene, this.left);
    this.quad.display();
    this.scene.translate(0, 0, 0.5);
    this.scene.rotate(0,degToRad(-90),0,1,0);
    this.scene.popMatrix();
  }
  
  lids() {
    this.scene.pushMatrix();
    this.scene.rotate(degToRad(90),1,0,0);
    this.scene.translate(0, 0, 0.5);
    this.applyTexture(this.scene, this.bottom);
    this.quad.display();
    this.scene.translate(0, 0, -1);
    this.applyTexture(this.scene, this.top);
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

