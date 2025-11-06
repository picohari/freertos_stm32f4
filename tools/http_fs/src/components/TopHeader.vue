<script setup>
defineProps({
  msg: {
    type: String,
    required: true
  }
})
</script>

<template>
  <div class="greetings">
    <h1 class="blue">{{ msg }}</h1>
    <h3>
      Firmware for  
      <a href="https://www.st.com/en/microcontrollers-microprocessors/stm32f407-417.html" target="_blank" rel="noopener">STM32F407i</a> +
      <a href="https://www.arm.com/technologies/cmsis" target="_blank" rel="noopener">CMSIS</a> +
      <a href="https://www.freertos.org/" target="_blank" rel="noopener">FreeRTOS</a> +
      <a href="https://savannah.nongnu.org/projects/lwip/" target="_blank" rel="noopener">lwIP</a> +
      <a href="https://ugfx.io/" target="_blank" rel="noopener">µGFX</a>
    </h3>
  </div>
</template>

<style scoped>
h1 {
  font-weight: 500;
  font-size: 2.6rem;
  position: relative;
  top: -10px;
}

h3 {
  font-size: 1.2rem;
}

.greetings h1,
.greetings h3 {
  text-align: center;
  margin: 0;
}

@media (min-width: 1024px) {
  .greetings h1,
  .greetings h3 {
    text-align: left;
  }
}
</style>
