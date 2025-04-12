import * as Phaser from 'phaser';

export type CarData =  {
    pos: Phaser.Math.Vector2,
    vel: Phaser.Math.Vector2,
    acc: Phaser.Math.Vector2,
    maxSpeed: number,
    dragCoefficient: number
    rotation: number;
}

export default class Car extends Phaser.GameObjects.GameObject {

}
