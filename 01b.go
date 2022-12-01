package main

import "os"      // https://pkg.go.dev/os
import "log"     // https://pkg.go.dev/log
import "strconv" // https://pkg.go.dev/strconv
import "sort"    // https://pkg.go.dev/sort
import "fmt"     // https://pkg.go.dev/fmt
import "bufio"   // https://pkg.go.dev/bufio

type ElfCalories = []int64

func main() {
	var elfCalories = readFromStdinAndParse()

	// Sort `elfCalories` descending.
	sort.Slice(elfCalories, func(i, j int) bool { return elfCalories[i] > elfCalories[j] })

	fmt.Println(sum(elfCalories[0:3]))
}

func sum(values []int64) int64 {
	var sum int64 = 0

	for _, value := range values {
		sum += value
	}

	return sum
}

func readFromStdinAndParse() ElfCalories {
	var elfCalories ElfCalories
	var currentCalories int64 = 0

	scanner := bufio.NewScanner(os.Stdin)

	for scanner.Scan() {
		line := scanner.Text()

		if line == "" {
			elfCalories = append(elfCalories, currentCalories)
			currentCalories = 0
		} else if line != "" {
			calories, err := strconv.ParseInt(line, 10, 64)

			if err != nil {
				log.Fatal(err)
			}

			currentCalories += calories
		}
	}

	if err := scanner.Err(); err != nil {
		log.Fatal(err)
	}

	if (currentCalories > 0) {
		elfCalories = append(elfCalories, currentCalories)
	}

	if len(elfCalories) < 3 {
		log.Fatal("Expected at least three elves")
	}

	return elfCalories
}