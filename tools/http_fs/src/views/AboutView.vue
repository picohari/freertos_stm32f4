<template>
  <div class="about">
    <h1>Project Info</h1>
    <p>Here are some commands needed to fully adapt the project:</p>



    <div>
      <h3>Create fonts for GUI</h3>
      <code>
        ./mcufont import_bdf HaxorMedium-12.bdf 
        ./mcufont bwfont_export HaxorMedium-12.dat haxor_12.c
      </code>
    </div>

    <div>
      <h3>Menu page generator</h3>
      <code>python generate_menus.py</code>
    </div>

    <div>
      <h3>Build mkfs for fs data</h3>
      <p>A PERL script named makefsdata already exists!</p>
      <code>gcc makefsdata.c -o mkfs</code>
    </div>




    <div>
      <h3>Setup Website</h3>
      <code>stm32-websocket$ npm install && npm run dev</code>
    </div>

    <div>
      <h3>Compile Website</h3>
      <code>stm32-websocket$ npm run compile</code>
    </div>

    <div>
      <h3>Compile Siimple CSS</h3>
      <code>stm32-websocket$ npx siimple -c ./siimple.config.js -o ./src/assets/siimple.css</code>
    </div>

    <div>
      <h3>Websocket Server for testing</h3>
      <code>stm32-websocket$ node test-ws.js</code>
    </div>



  </div>
</template>

<style>
@media (min-width: 1024px) {

}
</style>


