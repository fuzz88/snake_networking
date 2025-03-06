package main

import (
	"bytes"
	"encoding/binary"
	"fmt"
	"math/rand"
	"net"
	"sync"
	"time"
)

type Client struct {
	conn net.Conn
	id   int
}

type Point struct {
	X int8
	Y int8
}

type DataPacket struct {
	PacketType uint32
	PlayerID   uint32
	Score      uint32
	DataLen    uint32
	Data       []Point
}

var (
	clients   = make(map[int]*Client)
	clientsMu sync.Mutex
	clientID  = 0
)

func handleClient(conn net.Conn) {
	defer conn.Close()
	clientsMu.Lock()
	id := clientID
	fmt.Println("Connected ", id)
	clients[id] = &Client{conn: conn, id: id}
	clientID++
	clientsMu.Unlock()

	buf := make([]byte, 1024)
	for {
		n, err := conn.Read(buf)
		if err != nil {
			break
		}

		packetType := binary.BigEndian.Uint32(buf[0:4])
		playerID := binary.BigEndian.Uint32(buf[4:8])
		score := binary.BigEndian.Uint32(buf[8:12])
		dataLen := binary.BigEndian.Uint32(buf[12:16])

		var data []Point

		for i := 0; i < int(dataLen); i++ {
			offset := 12 + i*2
			data = append(data, Point{
				X: int8(buf[offset]),
				Y: int8(buf[offset+1]),
			})
		}

		packet := DataPacket{
			PacketType: packetType,
			PlayerID:   playerID,
			Score:      score,
			DataLen:    dataLen,
			Data:       data,
		}

		broadcast(buf[:n], id)
	}

	clientsMu.Lock()
	delete(clients, id)
	clientsMu.Unlock()
}

func generatePackets() {
	ticker := time.NewTicker(10 * time.Second)
	defer ticker.Stop()

	for range ticker.C {
		randomPoint := Point{X: int8(rand.Intn(30) + 5), Y: int8(rand.Intn(20) + 5)}
		packet := DataPacket{
			PacketType: 1,
			PlayerID:   1337,
			Score:      0,
			DataLen:    1,
			Data:       []Point{randomPoint},
		}
		fmt.Println("Generated apple packet:", packet)
		broadcast(encodePacket(&packet), -1)
	}
}

func encodePacket(packet *DataPacket) []byte {
	buf := new(bytes.Buffer)

	// Write fields in big-endian (matching C's ntohl/htonl)
	binary.Write(buf, binary.BigEndian, packet.PacketType)
	binary.Write(buf, binary.BigEndian, packet.PlayerID)
	binary.Write(buf, binary.BigEndian, packet.Score)
	binary.Write(buf, binary.BigEndian, packet.DataLen)

	// Write each Point (each is 2 bytes: X, Y)
	for _, point := range packet.Data {
		binary.Write(buf, binary.BigEndian, point)
	}

	return buf.Bytes()
}

func broadcast(data []byte, senderID int) {
	clientsMu.Lock()
	defer clientsMu.Unlock()

	for id, client := range clients {
		if id != senderID {
			fmt.Println("Sent to ", id)
			client.conn.Write(data)
		}
	}
}

func main() {
	ln, err := net.Listen("tcp", ":9000")
	if err != nil {
		fmt.Println("Error starting server:", err)
		return
	}
	defer ln.Close()

	fmt.Println("Server started on port 9000")

	go generatePackets()

	for {
		conn, err := ln.Accept()
		if err != nil {
			fmt.Println("Connection error:", err)
			continue
		}
		if len(clients) < 10 {
			go handleClient(conn)
		} else {
			continue
		}
	}
}
