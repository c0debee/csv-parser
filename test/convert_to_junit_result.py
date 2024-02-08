import xml.etree.ElementTree as ET

def convert_to_junit_xml(test_results):
    testsuite = ET.Element('testsuite', name='TestSuite', tests=str(len(test_results)), failures=str(count_failures(test_results)), errors='0', skipped='0')

    for result in test_results:
        test_case = ET.SubElement(testsuite, 'testcase', name=result['name'], classname='TestClass')
        if result['status'] == 'FAIL':
            failure = ET.SubElement(test_case, 'failure', message=result['message'])
        elif result['status'] == 'IGNORED':
            skipped = ET.SubElement(test_case, 'skipped')

    tree = ET.ElementTree(testsuite)
    tree.write('result/junit_results.xml', xml_declaration=True, encoding='utf-8')

def count_failures(test_results):
    return sum(1 for result in test_results if result['status'] == 'FAIL')

def read_test_results_from_file(file_path):
    test_results = []
    with open(file_path, 'r') as file:
        for line in file:
            parts = line.strip().split(':')
            if len(parts) > 3:
                name = parts[2]
                status = parts[3].lower()
                message = parts[4] if len(parts) > 4 else None

                # Check for PASS, FAIL, IGNORED keywords
                if "pass" in status:
                    status = 'PASS'
                elif "fail" in status:
                    status = 'FAIL'
                elif "ignore" in status:
                    status = 'IGNORED'

                result = {'name': name, 'status': status, 'message': message}
                test_results.append(result)
    return test_results

if __name__ == "__main__":
    # Specify the path to the input text file
    input_file_path = 'result/test_result'

    # Read test results from the input file
    test_results = read_test_results_from_file(input_file_path)

    # Convert to JUnit XML
    convert_to_junit_xml(test_results)
