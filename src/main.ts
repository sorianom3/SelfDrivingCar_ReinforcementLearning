import Phaser from "phaser";
import Simulation from "./Scenes/Simulation";

const config : Phaser.Types.Core.GameConfig = {
  type: Phaser.AUTO,
  backgroundColor: "#000000",
  title: "Self Driving AI",
  scale: {
      mode: Phaser.Scale.FIT,
      parent: 'phaser-container',
      autoCenter: Phaser.Scale.CENTER_BOTH,
      width: window.innerWidth,
      height: window.innerHeight,

  },
  scene: [
    Simulation
  ],

} 

export default new Phaser.Game(config);