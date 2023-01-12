document.getElementById("theme-toggle").addEventListener("click", () => {
    document.body.style.transition = "none";
    document.getElementById("header").style.transition = "none";
    if (document.body.className.includes("dark")) {
        localStorage.setItem("pref-theme", 'light');
        setLight();
    } else {
        localStorage.setItem("pref-theme", 'dark');
        setDark();
    }
    document.body.style.transition = "all 0.5s ease-in-out";
    document.getElementById("header").style.transition = "all 0.5s ease-in-out";
})

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

if (localStorage.getItem("pref-theme") === 'dark') {
    localStorage.setItem("pref-theme", 'dark');
    setDark();
} else {
    localStorage.setItem("pref-theme", 'light');
    setLight();
}
