package main

import (
	"bytes"
	"encoding/json"
	"encoding/xml"
	"fmt"
	"golang.org/x/net/html/charset"
	"io"
	"net/http"
)

type msg string

type ValCurs struct {
	XMLName xml.Name `xml:"ValCurs"`
	Date    string   `xml:"Date,attr"`
	Valute  []Valute `xml:"Valute"`
}

type Valute struct {
	ID       string `xml:"ID,attr"`
	CharCode string `xml:"CharCode"`
	Nominal  int    `xml:"Nominal"`
	Name     string `xml:"Name"`
	Value    string `xml:"Value"`
}

func request() ([]byte, error) {
	client := &http.Client{}
	req, err := http.NewRequest("GET", "https://www.cbr.ru/scripts/XML_daily.asp", nil)

	if err != nil {
		fmt.Println("Ошибка создания запроса:", err)
	}

	req.Header.Set("User-Agent", "Mozilla/5.0")
	resp, err := client.Do(req)

	if err != nil {
		fmt.Println("Ошибка запроса:", err)
	}

	defer resp.Body.Close()

	return io.ReadAll(resp.Body)
}

func xmlToJSON(xmlData []byte) (string, error) {
	var data ValCurs

	decoder := xml.NewDecoder(bytes.NewReader(xmlData))
	decoder.CharsetReader = charset.NewReaderLabel

	if err := decoder.Decode(&data); err != nil {
		return "", err
	}

	jsonData, err := json.MarshalIndent(data, "", "  ")
	if err != nil {
		return "", err
	}

	return string(jsonData), nil
}

func (m msg) ServeHTTP(resp http.ResponseWriter, req *http.Request) {
	fmt.Fprint(resp, m)
}

func main() {

	xml, err := request()
	json, err := xmlToJSON(xml)
	if err != nil {
		fmt.Println("Ошибка конвертации:", err)
		return
	}
	msgHandler := msg(json)

	fmt.Println("Server is listening...")
	http.ListenAndServe("localhost:8080", msgHandler)
}
