package main

import (
    "bytes"
    "fmt"
    "io"
    "net/http"
    "os"
)

func main() {
    if len(os.Args) < 3 {
        fmt.Println("Usage: mimoget <method> <url> [-d <data>] [-H <header>]")
        return
    }

    method := os.Args[1]
    url := os.Args[2]
    var body io.Reader
    var headers []string

    for i := 3; i < len(os.Args); i++ {
        switch os.Args[i] {
        case "-d":
            if i+1 < len(os.Args) {
                body = bytes.NewBufferString(os.Args[i+1])
                i++
            }
        case "-H":
            if i+1 < len(os.Args) {
                headers = append(headers, os.Args[i+1])
                i++
            }
        }
    }

    req, err := http.NewRequest(method, url, body)
    if err != nil {
        fmt.Println("[mimoget] ERROR:", err)
        return
    }

    for _, h := range headers {
        parts := bytes.SplitN([]byte(h), []byte(":"), 2)
        if len(parts) == 2 {
            req.Header.Set(string(bytes.TrimSpace(parts[0])), string(bytes.TrimSpace(parts[1])))
        }
    }

    resp, err := http.DefaultClient.Do(req)
    if err != nil {
        fmt.Println("[mimoget] ERROR:", err)
        return
    }
    defer resp.Body.Close()

    io.Copy(os.Stdout, resp.Body)
}
