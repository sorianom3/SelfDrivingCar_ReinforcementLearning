import Phaser from "phaser";
import Simulation from "./Scenes/Simulation";

const config : Phaser.Types.Core.GameConfig = {
  type: Phaser.AUTO,
  parent: "phaser-container",
  width: 800,
  height: 800,
  backgroundColor: "#000000",
  title: "Self Driving AI",
  scale: {
      mode: Phaser.Scale.FIT,
      autoCenter: Phaser.Scale.CENTER_BOTH
  },
  scene: [
    Simulation
  ],

} 

export default new Phaser.Game(config);