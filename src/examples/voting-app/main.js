let userInfo = {};

if (window.location.pathname.includes("index.html")) {
  document.getElementById("loginForm").onsubmit = async (e) => {
    e.preventDefault();
    const name = document.getElementById("name").value;
    const surname = document.getElementById("surname").value;
    const tc = document.getElementById("tc").value;
    const province = document.getElementById("province").value;
    const result = await window.pywebview.api.has_voted(tc);
    if(province.value == "") 
    {
        alert("Please Select City.");
        return;
    }

    if (result) 
    {
        alert("You are already voted. You can't vote again.");
    }
    else 
    {
      userInfo = { name, surname, tc, province };
      sessionStorage.setItem("user", JSON.stringify(userInfo));
      console.log("userInfo", userInfo);
      window.location.href = "vote.html";
    }
  };
}