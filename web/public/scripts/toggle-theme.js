document.getElementById("theme-toggle").addEventListener("click", () => {
    if (document.body.className.includes("dark")) {
        setLight();
        localStorage.setItem("pref-theme", 'light');
    } else {
        setDark();
        localStorage.setItem("pref-theme", 'dark');
    }
})

if (localStorage.getItem("pref-theme") === 'dark') {
    setDark();
} else {
    setLight();
}

function setDark(){
    document.body.classList.add('dark');
    document.getElementById("image-logo").src = "/images/logo/BIG-OKI-logo-1.png";
    document.getElementById("sun").style.display = "block";
    document.getElementById("moon").style.display = "none";
}

function setLight(){
    document.body.classList.remove('dark');
    document.getElementById("image-logo").src = "/images/logo/BIG-OKI-logo-2.png";
    document.getElementById("sun").style.display = "none";
    document.getElementById("moon").style.display = "block";
}
