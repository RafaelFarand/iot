<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>IoT Real-Time Monitoring</title>
    <link rel="stylesheet" href="css/style.css">
</head>
<body>
    <header>
        <h1>Real-Time Monitoring</h1>
    </header>
    <main>
        <div class="data-container">
            <h2>Data Sensor</h2>
            <p>Kelembapan Tanah: <span id="kelembapan"></span></p>
            <p>Cahaya: <span id="cahaya"></span></p>
            <p>Timestamp: <span id="timestamp"></span></p>
        </div>
    </main>
    <footer>
        <p>© 2024 IoT Monitoring by PANDAWA FARM</p>
    </footer>
    <script>
        // Fungsi untuk mengambil data terbaru dari server
        function fetchData() {
            fetch('php/receive_and_fetch_data.php')
                .then(response => response.json())
                .then(data => {
                    if (data.status === "success") {
                        document.getElementById('kelembapan').textContent = data.data.kelembapan;
                        document.getElementById('cahaya').textContent = data.data.cahaya == 1 ? 'Kurang' : 'Cukup';
                        document.getElementById('timestamp').textContent = data.data.timestamp;
                    } else {
                        console.error("Error fetching data:", data.message);
                    }
                })
                .catch(error => console.error("Error:", error));
        }

        // Panggil fetchData setiap 5 detik
        setInterval(fetchData, 5000);
        fetchData(); // Panggilan awal saat halaman dimuat
    </script>
</body>
</html>
