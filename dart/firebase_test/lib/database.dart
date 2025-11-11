import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:firebase_test/someclass.dart';

abstract interface class DBI {
  set pushSomething(Something data);
  Future<Iterable<Something>> get getWhatever;
}

class Database implements DBI {
  final FirebaseFirestore _db;
  String _docId;

  Database(this._docId)
    : _db = FirebaseFirestore.instance;

  set setDocId(String newId) {_docId = newId;}
  String get getDocId => _docId;

  @override set pushSomething(Something data) {
    Map<String, dynamic> tmp = {};
    tmp[Values.v1.getName] = data.getval1;
    tmp[Values.v2.getName] = data.getval2;
    tmp[Values.v3.getName] = data.getval3;

    _db.doc(_docId).set(tmp);
  }

  @override Future<Iterable<Something>> get getWhatever async {
    List<Something> things = <Something>[];
    DocumentReference df = _db.doc(getDocId);

    await df.get().then(
      (DocumentSnapshot doc) {
        final data = doc.data() as Map<String, dynamic>;
        for(String curKey in data.keys) {
          print("key: $curKey, val: ${data[curKey]}");
        }
        things.add(Something.factory(data));
      },
      onError: (e) => print("Could not get doc \"$getDocId\"")
    );

    return things;
  }
}