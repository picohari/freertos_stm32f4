<template>
    <div class="wrapper">
        <div class="version">
            <p>This website has been built with ❤️ using <a href="https://vite.dev/">Vite</a> + <a href="https://vuejs.org/">Vue3</a></p>
            <p>All code of this project is licenced under the <a href="http://www.gnu.org/licenses/">GNU General Public License</a>unless otherwise stated.</p>
            <p>Current version: <!--#version--></p>
        </div>
        <div class="legal">
            <ul>
                <li><a href="#">Github Project</a></li>
                <li><a href="#">DK6YF Blog</a></li>
            </ul>
            <p>© 2025 - H9 Laboratory Ltd. All rights reserved.</p>
        </div>
    </div>
</template>
      
<script setup>
// No script needed yet
</script>
    
<style scoped>

.version {
//display: flex;
gap: 1rem;
}

.foot-link {
text-decoration: none;
font-weight: 600;
}

.foot-link:hover {
text-decoration: underline;
}

.legal {
    text-align: center;
}

.legal ul {
    list-style: none;
}

.legal li {
    display: inline-block;
}


@media (min-width: 1024px) {
  .version {
    text-align: left;
  }

  .legal {
    text-align: right;
  }

  .legal li {
    display: block;
  }

}

</style>