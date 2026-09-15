(function () {
  var STORAGE_KEY = 'connix-theme';
  var btn = document.getElementById('theme-toggle');
  var icon = btn ? btn.querySelector('.theme-icon') : null;

  function systemPrefersDark() {
    return window.matchMedia && window.matchMedia('(prefers-color-scheme: dark)').matches;
  }

  function getStoredTheme() {
    try { return localStorage.getItem(STORAGE_KEY); } catch (e) { return null; }
  }

  function storeTheme(theme) {
    try { localStorage.setItem(STORAGE_KEY, theme); } catch (e) {}
  }

  function applyTheme(theme) {
    document.documentElement.setAttribute('data-theme', theme);
    if (icon) icon.textContent = theme === 'dark' ? '☾' : '☀';
  }

  function resolveInitialTheme() {
    var stored = getStoredTheme();
    if (stored === 'dark' || stored === 'light') return stored;
    return systemPrefersDark() ? 'dark' : 'light';
  }

  // Apply before paint to avoid flash
  applyTheme(resolveInitialTheme());

  if (btn) {
    btn.addEventListener('click', function () {
      var current = document.documentElement.getAttribute('data-theme');
      var next = current === 'dark' ? 'light' : 'dark';
      applyTheme(next);
      storeTheme(next);
    });
  }

  // Sync with OS preference changes when no stored override
  if (window.matchMedia) {
    window.matchMedia('(prefers-color-scheme: dark)').addEventListener('change', function (e) {
      if (!getStoredTheme()) applyTheme(e.matches ? 'dark' : 'light');
    });
  }
})();
