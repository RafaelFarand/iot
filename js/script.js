setInterval(() => {
    fetch('php/fetch_data.php')
        .then(response => response.json())
        .then(data => {
            document.getElementById('kelembapan').textContent = data.kelembapan || '-';
            document.getElementById('ph').textContent = data.ph || '-';
        })
        .catch(error => console.error('Error fetching data:', error));
}, 5000); // Update setiap 5 detik
