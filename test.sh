result=$(git status --porcelain)
result2=$(echo $result | wc -l)

echo $result
echo $result2

if [[ -n $result ]]; then
  echo "dirty=👎🏻"
else
  echo "dirty=👍🏻"
fi
