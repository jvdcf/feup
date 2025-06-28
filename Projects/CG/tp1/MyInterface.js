import {CGFinterface, dat} from '../lib/CGF.js';

/**
* MyInterface
* @constructor
*/
export class MyInterface extends CGFinterface {
    constructor() {
        super();
    }

    init(application) {
        // call CGFinterface init
        super.init(application);
        
        // init GUI. For more information on the methods, check:
        // https://github.com/dataarts/dat.gui/blob/master/API.md
        this.gui = new dat.GUI();

        //Checkbox element in GUI
        this.gui.add(this.scene, 'displayAxis').name('Display Axis');

        //Slider element in GUI
        this.gui.add(this.scene, 'scaleFactor', 0.1, 5).name('Scale Factor');

        // Object checkboxes
        this.gui.add(this.scene, 'diamondCheck').name('Show Diamond');
        this.gui.add(this.scene, 'triangleCheck').name('Show Triangle');
        this.gui.add(this.scene, 'parallelogramCheck').name('Show Parallelogram');
        this.gui.add(this.scene, 'triangleSmallCheck').name('Show Small Triangle');
        this.gui.add(this.scene, 'triangleBigCheck').name('Show Big Triangle');

        return true;
    }
}