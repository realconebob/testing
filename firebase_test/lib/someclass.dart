import 'dart:collection';

enum Values {
  v1("val1", "an integer"),
  v2("val2", "a double"),
  v3("val3", "a string");

  final String _name, _desc;
  const Values(this._name, this._desc);
  @override String toString() => "Value: $_name $_desc";
  String get getName => _name;
}

abstract interface class ISomething {
  set setval1(int val);
  set setval2(double val);
  set setval3(String val);
  set setAllValues(Map<Values, dynamic> values);

  int get getval1;
  double get getval2;
  String get getval3;
  Iterable<dynamic> get getAllValues;
}

class Something implements ISomething {
  final Map<Values, dynamic> _map;

  Something(int v1, double v2, String v3): _map = SplayTreeMap() {
    setval1 = v1;
    setval2 = v2;
    setval3 = v3;
  }

  factory Something.factory(Map<String, dynamic> values) {
    int v1 = values[Values.v1.getName];
    double v2 = values[Values.v2.getName];
    String v3 = values[Values.v3.getName];

    return Something(v1, v2, v3);
  }

  @override set setval1(int val)    {_map[Values.v1] = val.abs();}
  @override set setval2(double val) {_map[Values.v2] = val.roundToDouble();}
  @override set setval3(String val) {_map[Values.v3] = val.trim();}
  @override set setAllValues(Map<Values, dynamic> values) {
    _map.clear();
    _map.addAll(values);
  }

  @override int get getval1 => _map[Values.v1];
  @override double get getval2 => _map[Values.v2];
  @override String get getval3 => _map[Values.v3];
  @override Iterable<dynamic> get getAllValues => _map.values;
}