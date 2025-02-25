package main

import (
	"fmt"
	"net"
	"sync"
)

type Client struct {
	conn net.Conn
	id   int
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
		broadcast(buf[:n], id)
	}

	clientsMu.Lock()
	delete(clients, id)
	clientsMu.Unlock()
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

	for {
		conn, err := ln.Accept()
		if err != nil {
			fmt.Println("Connection error:", err)
			continue
		}
		if len(clients) < 2 {
			go handleClient(conn)
		} else {
			continue
		}
	}
}
