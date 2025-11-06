<script setup>
//import "./assets/siimple.css"
import { RouterLink, RouterView } from 'vue-router'
import Projectname from './components/TopHeader.vue'
import NavHeader from './components/NavHeader.vue'
import NavFooter from './components/Footer.vue'
</script>

<template>

  <header class="top-header">
    <img alt="CPU logo" class="logo" src="@/assets/arm_cpu.svg" width="125" height="125" />

    <div class="wrapper">
      <Projectname msg="XCore407i" />
    </div>
  </header>

  <NavHeader />

  <main>
    <RouterView />
  </main>

  <footer class="footer">
    <NavFooter />
  </footer>

</template>


<style scoped>

main {
  padding: 2rem;
  margin-left: 2rem;
}

header {
  line-height: 1.5;
  max-height: 100vh;
}

.logo {
  display: block;
  margin: 0 auto 2rem;
}

nav {
  width: 100%;
  font-size: 12px;
  text-align: center;
  margin-top: 2rem;
}

nav a.router-link-exact-active {
  color: var(--color-text);
}

nav a.router-link-exact-active:hover {
  background-color: transparent;
}

nav a {
  display: inline-block;
  padding: 0 1rem;
  border-left: 1px solid var(--color-border);
}

nav a:first-of-type {
  border: 0;
}

.footer {
  background: #f2f2f2; /* slate-800 */
  padding: 1rem;
  font-size: 0.85em;
  font-weight: 800;
  text-align: center;
}

@media (min-width: 1024px) {
  .logo {
    margin: 0 2rem 0 0;
  }

  header {
    display: flex;
    place-items: center;
    padding-right: calc(var(--section-gap) / 2);
  }

  header .wrapper {
    display: flex;
    place-items: flex-start;
    flex-wrap: wrap;
  }

  nav {
    text-align: left;
    margin-left: -1rem;
    font-size: 1rem;

    padding: 1rem 0;
    margin-top: 1rem;
  }

}

</style>