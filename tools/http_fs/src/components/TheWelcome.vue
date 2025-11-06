<script setup>
import WelcomeItem from './WelcomeItem.vue'
import FlowchartIcon from './icons/IconFlowchart.vue'
import ToolingIcon from './icons/IconTooling.vue'
import EcosystemIcon from './icons/IconEcosystem.vue'
import SystemIcon from './icons/IconSystem.vue'
import SupportIcon from './icons/IconInfo.vue'
</script>

<template>

  <WelcomeItem>
    <template #icon>
      <SystemIcon />
    </template>
    <template #heading>IO System</template>

    Full implemented HAL, TCP-IP and GFX on TouchScreen LCD for XCore board with STM32F407VTG processor.
    OS provided by <a href="https://www.freertos.org/" target="_blank" rel="noopener">FreeRTOS</a>.
    See <a href="https://www.github.com/picohari" target="_blank" rel="noopener">official documentation</a>and check latest versions.
  </WelcomeItem>

  <WelcomeItem>
    <template #icon>
      <ToolingIcon />
    </template>
    <template #heading>Tooling</template>

    This webpage project is served and bundled with
    <a href="https://vitejs.dev/guide/features.html" target="_blank" rel="noopener">Vite</a>. The
    recommended IDE setup is
    <a href="https://code.visualstudio.com/" target="_blank" rel="noopener">VSCode</a> +
    <a href="https://github.com/johnsoncodehk/volar" target="_blank" rel="noopener">Volar</a>.
    
    Compilation of firmware as <code>makefile</code> project. You need external programming hardware to flash memory on microcontroller.
    More instructions are available in <code>README.md</code>.
  </WelcomeItem>

  <WelcomeItem>
    <template #icon>
      <EcosystemIcon />
    </template>
    <template #heading>Ecosystem</template>

    Get official tools and libraries for your project:
    <a href="https://ubuntu.com/" target="_blank" rel="noopener">Ubuntu</a>,
    <a href="https://github.com/" target="_blank" rel="noopener">GitHub</a>,
    <a href="https://test-utils.vuejs.org/" target="_blank" rel="noopener">Vue Test Utils</a>, and
    <a href="https://github.com/vuejs/devtools" target="_blank" rel="noopener">Vue Dev Tools</a>. If
    you need more resources, we suggest paying
    <a href="https://github.com/vuejs/awesome-vue" target="_blank" rel="noopener">Awesome Vue</a>
    a visit.
  </WelcomeItem>

  <WelcomeItem>
    <template #icon>
      <FlowchartIcon />
    </template>
    <template #heading>Network Applications</template>

    Full supported <a href="https://savannah.nongnu.org/projects/lwip/" target="_blank" rel="noopener">lwIP</a> TCP-IP stack featuring
    multiple applications using TCP-IP and UDP. Also IPv6 is configurable. 
  </WelcomeItem>

  <WelcomeItem>
    <template #icon>
      <SupportIcon />
    </template>
    <template #heading>Help and Support</template>

    As an independent project, this project relies on community backing for its sustainability. You can help
    us by <a href="https://vuejs.org/sponsor/" target="_blank" rel="noopener">becoming a sponsor</a>.
  </WelcomeItem>
</template>
