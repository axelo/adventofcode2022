package main

import "os"      // https://pkg.go.dev/os
import "log"     // https://pkg.go.dev/log
import "strings" // https://pkg.go.dev/strings
import "strconv" // https://pkg.go.dev/strconv
import "sort"    // https://pkg.go.dev/sort#Sort

type ElfCalories = []int64

func main() {
	var elfCalories = parse(readInput())

	// Sort `elfCalories` descending.
	sort.Slice(elfCalories, func(i, j int) bool { return elfCalories[i] > elfCalories[j] })

	bigElves := elfCalories[0:3]

	log.Printf("Most calories : %d", bigElves[0])
	log.Printf("Total calories of the three elves: %d", sum(bigElves))
}

func inputFilepath() string {
	if os.Getenv("EXAMPLE") == "1" {
		return "day01.example.txt"
	} else {
		return "day01.txt"
	}
}

func readInput() string {
	inputBytes, err := os.ReadFile(inputFilepath())

	if err != nil {
		log.Fatal(err)
	}

	return string(inputBytes)
}

func parse(input string) ElfCalories {
	lines := strings.Split(input, "\n")
	lines = append(lines, "")

	var calories int64 = 0
	var elfCalories ElfCalories

	for _, line := range lines {
		if line == "" {
			elfCalories = append(elfCalories, calories)

			calories = 0
		} else if line != "" {
			c, err := strconv.ParseInt(line, 10, 64)

			if err != nil {
				log.Fatal(err)
			}

			calories += c

		}
	}

	if len(elfCalories) < 3 {
		log.Fatal("Expected at least three elves")
	}

	return elfCalories
}

func sum(values []int64) int64 {
	var sum int64 = 0

	for _, value := range values {
		sum += value
	}

	return sum
}
