<?php
// Konfigurasi database
$servername = "localhost";
$username = "root";
$password = "";
$dbname = "iot_data";

// Koneksi ke database
$conn = new mysqli($servername, $username, $password, $dbname);

// Periksa koneksi
if ($conn->connect_error) {
    die(json_encode([
        "status" => "error",
        "message" => "Database connection failed: " . $conn->connect_error
    ]));
}

// Jika menerima data POST dari ESP32
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $kelembapan = $_POST['kelembapan'] ?? null;
    $cahaya = $_POST['cahaya'] ?? null;

    if ($kelembapan !== null && $cahaya !== null) {
        // Masukkan data ke database
        $stmt = $conn->prepare("INSERT INTO sensor_data (kelembapan, cahaya) VALUES (?, ?)");
        $stmt->bind_param("ii", $kelembapan, $cahaya);

        if ($stmt->execute()) {
            echo json_encode([
                "status" => "success",
                "message" => "Data saved successfully"
            ]);
        } else {
            echo json_encode([
                "status" => "error",
                "message" => "Failed to save data: " . $stmt->error
            ]);
        }
        $stmt->close();
    } else {
        echo json_encode([
            "status" => "error",
            "message" => "Invalid data received"
        ]);
    }
    exit();
}

// Jika menerima permintaan GET untuk mengambil data terbaru
if ($_SERVER['REQUEST_METHOD'] === 'GET') {
    $sql = "SELECT kelembapan, cahaya, timestamp FROM sensor_data ORDER BY timestamp DESC LIMIT 1";
    $result = $conn->query($sql);

    if ($result->num_rows > 0) {
        $row = $result->fetch_assoc();
        echo json_encode([
            "status" => "success",
            "data" => $row
        ]);
    } else {
        echo json_encode([
            "status" => "error",
            "message" => "No data found"
        ]);
    }
    exit();
}

// Tutup koneksi database
$conn->close();
?>
