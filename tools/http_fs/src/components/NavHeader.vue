<template>
<header class="nav-header">
    <nav class="nav-left">
        <RouterLink to="/" class="nav-link">Home</RouterLink>
        <RouterLink to="/websocket" class="nav-link">Websockets</RouterLink>
        <RouterLink to="/about" class="nav-link">About</RouterLink>
    </nav>
    <nav class="nav-right">
        <a href="/tasks.html" class="nav-link">Tasks</a>
    </nav>
</header>
</template>
  
<script setup>
// No script needed yet
</script>

<style scoped>
.nav-header {
    background: #1e293b; /* slate-800 */
    padding: 1rem;
}

.nav-left {
    display: flex;
    gap: 1rem;
}

.nav-right {
  margin-left: auto;
}

.nav-link {
    color: #ffffff;
    text-decoration: none;
    font-weight: 600;
}

.nav-link:hover {
    text-decoration: underline;
}


</style>
  