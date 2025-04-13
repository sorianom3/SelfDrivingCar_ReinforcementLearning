import * as Phaser from 'phaser';
import Track from './Track';
import { Math } from 'phaser';

export type CarData =  {
    pos: Math.Vector2,
    vel: Math.Vector2,
    acc: Math.Vector2,
    maxSpeed: number,
    dragCoefficient: number
    rotation: number;
}

export default class Car extends Phaser.GameObjects.GameObject{
    
    public car : CarData = {
        pos: new Math.Vector2(0,0),
        vel: new Math.Vector2(0,0),
        acc : new Math.Vector2(0,0),
        maxSpeed : 10,
        dragCoefficient: 0.95,
        rotation : 90
    }
    private trackData : Track;
    private carGraphic: Phaser.GameObjects.Graphics;

    constructor(scene: Phaser.Scene, track: Track){
        super(scene, 'Car');
        this.trackData = track;
        this.carGraphic = scene.make.graphics(this.car.pos,false);

        this.scene.make.graphics()
    }
}
