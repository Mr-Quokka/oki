/**
 * @param {'dark'|'light'} colorPreference
 */
function updateSourceMedia(colorPreference) {
    document.querySelectorAll('picture').forEach((picture) => {
        const sources = picture.querySelectorAll(`
        source[media*="prefers-color-scheme"], 
        source[data-media*="prefers-color-scheme"]
      `);

        sources.forEach((source) => {
            if (source.media.includes('prefers-color-scheme')) {
                source.dataset.media = source.media;
            }
            if (source.dataset.media.includes(colorPreference)) {
                source.media = 'all';
            } else if (source) {
                source.media = 'none';
            }
        })
    })
}

document.getElementById('theme-toggle').addEventListener('click', () => {
    if (document.body.classList.toggle('dark')) {
        updateSourceMedia('dark');
        localStorage.setItem('pref-theme', 'dark');
    } else {
        updateSourceMedia('light');
        localStorage.setItem('pref-theme', 'light');
    }
});
window.addEventListener('load', () => document.body.classList.remove('preload'));
updateSourceMedia(document.body.classList.contains('dark') ? 'dark' : 'light');
