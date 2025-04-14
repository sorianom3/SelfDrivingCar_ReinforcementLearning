import * as Phaser from "phaser"
import Car from '../GameObjects/Car';
import Track from '../GameObjects/Track';

export default class Simulation extends Phaser.Scene {

    car : Car | undefined;
    track : Track | undefined;
    create(){
        this.track = new Track(this);
        this.car = new Car(this, this.track);

    }
    
    update(_time: number, _delta: number): void {
        this.car?.collectPlayerInput();
        this.car?.update(_delta * 0.001);
    }
}