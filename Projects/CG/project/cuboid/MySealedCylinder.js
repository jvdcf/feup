import { CGFobject } from "../../lib/CGF.js";
import { MyCylinder } from "./MyCylinder.js";
import { MyCircle } from "../2d/MyCircle.js"; 


export class MySealedCylinder extends CGFobject {
  constructor(scene, slices) {
    super(scene);
    this.scene = scene;
    this.slices = slices;
    this.cyl = new MyCylinder(scene, slices, 1);
    this.cap = new MyCircle(scene, 1., slices);
  }

  display() {
    this.scene.pushMatrix();
    this.cyl.display();

    this.scene.pushMatrix();
    this.scene.translate(0,0,1);  
    this.cap.display();
    this.scene.popMatrix();
    
    this.scene.pushMatrix();
    this.scene.rotate(Math.PI, 1, 0, 0);
    this.cap.display();
    this.scene.popMatrix();
    
    this.scene.popMatrix();
  }
  
}
