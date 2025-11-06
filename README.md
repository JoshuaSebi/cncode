# 🌐 Computer Networks Lab Programs

This repository contains **C programs for major Computer Networks experiments**, implemented using **Socket Programming** concepts.  
Each section includes the **algorithm** and corresponding **client–server code**, making it easy for quick revision during **placement preparation** or lab exams.

---

## 📘 Table of Contents
1. [TCP Client-Server Communication](#1-tcp-client-server)
2. [UDP Client-Server Communication](#2-udp-client-server)
3. [Armstrong Number Check using TCP](#3-armstrong-number-using-tcp)
4. [Time Server using TCP](#4-time-server-using-tcp)
5. [File Transfer (FTP) using TCP](#5-file-transfer-using-tcp)
6. [Stop and Wait ARQ (UDP)](#6-stop-and-wait-arq)
7. [Go Back N ARQ (UDP)](#7-go-back-n-arq)
8. [Selective Repeat ARQ (UDP)](#8-selective-repeat-arq)

---

## 1️⃣ TCP Client-Server

### 📋 **Algorithm**
**Server:**
1. Create a TCP socket using `socket()`.
2. Bind it to a port and IP using `bind()`.
3. Listen for connections using `listen()`.
4. Accept a client connection with `accept()`.
5. Receive a message from the client using `recv()`.
6. Send a reply using `send()`.
7. Close the connection.

**Client:**
1. Create a socket using `socket()`.
2. Connect to the server using `connect()`.
3. Send data to the server using `send()`.
4. Receive reply using `recv()`.
5. Close the socket.

---

## 2️⃣ UDP Client-Server

### 📋 **Algorithm**
**Server:**
1. Create a UDP socket.
2. Bind it to an IP and port.
3. Wait for data using `recvfrom()`.
4. Display received data.
5. Send a reply using `sendto()`.

**Client:**
1. Create a UDP socket.
2. Send message to server using `sendto()`.
3. Receive server reply using `recvfrom()`.
4. Close the socket.

---

## 3️⃣ Armstrong Number using TCP

### 📋 **Algorithm**
**Server:**
1. Create socket, bind, and listen.
2. Accept client connection.
3. Receive a number from client.
4. Compute Armstrong number:
   - Extract each digit.
   - Cube and sum all digits.
   - If sum equals the number, it’s an Armstrong number.
5. Send result (“Armstrong” or “Not Armstrong”) to client.
6. Repeat for multiple clients.

**Client:**
1. Create socket and connect to server.
2. Send number to check.
3. Receive and print result.
4. Close the socket.

---

## 4️⃣ Time Server using TCP

### 📋 **Algorithm**
**Server:**
1. Create TCP socket.
2. Bind and listen on a port.
3. Accept client connection.
4. On receiving a request, use `time()` and `ctime()` to get current time.
5. Send the current time to the client.
6. Continue for multiple clients.

**Client:**
1. Create TCP socket.
2. Connect to server.
3. Send time request message.
4. Receive current time and display.
5. Close socket.

---

## 5️⃣ File Transfer using TCP

### 📋 **Algorithm**
**Server:**
1. Create socket, bind, and listen.
2. Accept connection from client.
3. Receive file content from client.
4. Store content in a file (e.g., `abc.txt`).
5. Count number of vowels in received content.
6. Send confirmation message back to client.

**Client:**
1. Create TCP socket and connect.
2. Take filename as input.
3. Read file content.
4. Send file content to server.
5. Receive confirmation message.

---

## 6️⃣ Stop and Wait ARQ

### 📋 **Algorithm**
**Sender (Client):**
1. Create UDP socket.
2. Send frame with frame number.
3. Wait for ACK within timeout.
4. If ACK not received, resend the frame.
5. Repeat until all frames are sent.

**Receiver (Server):**
1. Create and bind UDP socket.
2. Receive frames.
3. Randomly drop some frames to simulate loss.
4. If correct frame received, send ACK.
5. Continue till all frames received.

---

## 7️⃣ Go Back N ARQ

### 📋 **Algorithm**
**Sender (Client):**
1. Create UDP socket.
2. Send frames within window size.
3. If timeout occurs, resend all frames from the unacknowledged frame.
4. Move window forward after receiving ACKs.

**Receiver (Server):**
1. Create and bind UDP socket.
2. Receive frames in order.
3. Drop one frame intentionally to simulate error.
4. Send cumulative ACK for last correctly received frame.

---

## 8️⃣ Selective Repeat ARQ

### 📋 **Algorithm**
**Sender (Client):**
1. Send frames within window size.
2. Maintain an ACK array for tracking received acknowledgements.
3. If timeout occurs, resend only missing (unacknowledged) frames.
4. Slide window when ACKs are received.

**Receiver (Server):**
1. Receive frames in any order.
2. Drop one frame intentionally to simulate packet loss.
3. Send ACK for every correctly received frame.
4. Maintain buffer for received frames until all are received.

---

## ⚙️ **How to Compile & Run**

### TCP/UDP Programs
```bash
gcc server.c 
gcc client.c 
./server
./client
