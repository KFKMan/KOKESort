const HOVER_COLOR = "#EFAE88";
const MAP_COLOR = "#fff2e3";

let infoName = document.getElementById("info-name");
const container = document.getElementById("candidates_container");
const presidentContainer = document.getElementById("presidents_container");

function waitForAPI() {
  return new Promise((resolve) => {
    const interval = setInterval(() => {
      if (window.pywebview && window.pywebview.api) {
        clearInterval(interval);
        resolve();
      }
    }, 100);
  });
}


async function initMap() {
  await waitForAPI(); // wait for API to be ready

  d3.json("cities.json").then(function (data) {
    let width = 1200;
    let height = 800;
    let projection = d3.geoEqualEarth();
    projection.fitSize([width, height], data);
    let path = d3.geoPath().projection(projection);

    let svg = d3
      .select("#map_container")
      .append("svg")
      .attr("viewBox", `0 0 ${width} ${height}`)
      .attr("preserveAspectRatio", "xMidYMid meet")
      .classed("responsive-svg", true);

    let g = svg
      .append("g")
      .selectAll("path")
      .data(data.features)
      .join("path")
      .attr("d", path)
      .attr("fill", MAP_COLOR)
      .attr("stroke", "#000")
      .on("mouseover", function () {
        d3.select(this).attr("fill", HOVER_COLOR);
      })
      .on("mouseout", function (d) {
        if (!d.noFill) d3.select(this).attr("fill", MAP_COLOR);
      })
      .on("click", async function (d, i) {
        let provinceName = d.properties.name;
        console.log("City Clicked on:", provinceName);
        infoName.innerText = "City: " + provinceName;
        container.innerHTML = "<p>Loading...</p>";

        try {
          const data = await window.pywebview.api.get_candidates(provinceName);

          console.log("Candidate Data:", data);
          container.innerHTML = ""; // Clear
          presidentContainer.innerHTML = ""; // Clear

          if (data && data.mayor) 
          {
            for (let key in data.mayor) {
              const div = document.createElement("div");
              let fullName = data.mayor[key].name;
              let voteCount = data.mayor[key].voteCount;
              div.className = "candidate-card";
              div.innerHTML = `
                <img src="images/default.jpg" alt="${fullName}">
                <h4>${fullName}</h4>
                <p>Vote Count: ${voteCount || 0}</p>
              `;
              container.appendChild(div);
            }
          } 
          else 
          {
            container.innerHTML = "<p>No candidate found</p>";
          }

          if (data && data.president) 
          {
            for (let key in data.president) {
              const div = document.createElement("div");
              let fullName = data.president[key].name;
              let voteCount = data.president[key].voteCount;
              div.className = "candidate-card";
              div.innerHTML = `
                <img src="images/default.jpg" alt="${fullName}">
                <h4>${fullName}</h4>
                <p>Vote Count: ${voteCount || 0}</p>
              `;
              presidentContainer.appendChild(div);
            }
          } 
        } catch (err) {
          console.error("Error accoured on getting candidates:", err);
          container.innerHTML = "<p>Error accoured.</p>";
        }
      });

    // Şehir isimlerini ekle
    const textGroup = svg.append("g");
    textGroup.selectAll("text")
      .data(data.features)
      .enter()
      .append("text")
      .text(d => d.properties.name)
      .attr("x", d => path.centroid(d)[0])
      .attr("y", d => path.centroid(d)[1])
      .attr("text-anchor", "middle")
      .attr("font-size", "10pt")
      .attr("pointer-events", "none");
  });
}

initMap();