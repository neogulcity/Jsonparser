Case #0: 단순한 데이터 타입

```json
{
    "myInt": 1,
    "myString": "text",
    "myBool": true
}
```

Case #0: 객체 데이터 타입

```json
{
    "myObject": {
        "myMemberInt": 1,
        "myMemberString": "text"
    }
}
```

Case #0: 이중 객체 데이터 타입

```json
{
    "myParentObject": {
        "myParentMemberInt": 1,
        "myParentMemberString": "text",
        "myChildObject": {
            "myChildMemberInt": 1,
            "myChildMemberString": "text"
        }
    }
}
```

Case #0: 배열 형태 단순한 데이터 타입

```json
{
    "myArray": [
        "1",
        "2",
        "3"
    ]
}
```

Case #0: 배열 형태 객체 데이터 타입

```json
{
    "myArray": [
        {
            "myMemberInt": 1,
            "myMemberString": "text"
        },
        {
            "myMemberInt": 1,
            "myMemberString": "text"
        }
    ]
}
```












=============================================
=============================================

Case ##: 이중 배열 단순한 데이터 타입
    >> Json에서는 볼 수 없는 형태 추후 판단.

```json
{
    "myArray": [
        "[0][0]",
        "[0][1]",
        "[1][0]",
        "[1][1]"
    ]
}
```